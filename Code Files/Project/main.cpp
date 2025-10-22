// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

#include "ass1.h"

#include "ImGuiFileDialog.h"


using namespace std;

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void LoadImage(Image& image, string imageName) //A function to help load an image to the ram and create its Shader Resource View
{
    // Free previous SRV if it exists
    if (image.srv)
    {
        image.srv->Release();
        image.srv = nullptr;
    }

    // Load image in RAM
    image.loadNewImage(imageName);

    // Describe texture for direct X
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = image.width;
    desc.Height = image.height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = image.imageData;
    initData.SysMemPitch = image.width * 4;

    // Create texture
    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &initData, &texture);
    if (FAILED(hr))
    {
        std::cerr << "Creating texture Failed";
        return;
    }

    // Create SRV
    hr = g_pd3dDevice->CreateShaderResourceView(texture, nullptr, &image.srv);
    texture->Release(); // Safe release

    if (FAILED(hr))
    {
        std::cerr << "Failed to create SRV\n";
        image.srv = nullptr;
    }
}
void UpdateImage(Image& image)//A function to help update the image after modifying its pixel data
{
    if (!image.imageData || !g_pd3dDevice) return;

    // Free previous SRV safely
    if (image.srv)
    {
        image.srv->Release();
        image.srv = nullptr;
    }

    // Recreate texture from updated pixel data
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = image.width;
    desc.Height = image.height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = image.imageData;
    initData.SysMemPitch = image.width * 4;

    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &initData, &texture);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create texture\n";
        return;
    }

    hr = g_pd3dDevice->CreateShaderResourceView(texture, nullptr, &image.srv);
    texture->Release();

    if (FAILED(hr))
    {
        std::cerr << "Failed to create SRV\n";
        image.srv = nullptr;
        return;
    }
}
string OpenFileDialog() //Opens file explorer to choose an image
{
    char filename[MAX_PATH] = "";

    //Built in DirectX stuff
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr; // or your window handle
    ofn.lpstrFilter = "Image Files\0*.png;*.jpg;*.jpeg;*.bmp;*.tga\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = "png";

    if (GetOpenFileNameA(&ofn))
    {
        return filename; // user selected a file
    }
    else
        return "";       // user canceled
}
std::string OpenSaveFileDialog() //same but for saving
{
    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter =
        "PNG Files (*.png)\0*.png\0"
        "JPEG Files (*.jpg;*.jpeg)\0*.jpg;*.jpeg\0"
        "BMP Files (*.bmp)\0*.bmp\0"
        "TGA Files (*.tga)\0*.tga\0"
        "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = "png"; // default extension

    if (GetSaveFileNameA(&ofn))
        return std::string(filename);

    return ""; // user cancelled
}

// Main code
int main(int, char**)
{
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Pixplode", WS_OVERLAPPEDWINDOW, 100, 100, (int)(1920 * main_scale), (int)(1080 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls



    ImGui::GetIO().FontGlobalScale = 1.2f; // scale once
    const char* fontPath = "fonts/PlayfairDisplay-VariableFont_wght.ttf";
    ImFont* myFont = io.Fonts->AddFontFromFileTTF(fontPath, 20.0f);
    if (myFont)
    {
        io.FontDefault = myFont;
    }

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


    // Our state
    bool show_demo_window = true;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;

    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);

    bool open = true;
    bool checkBox = false;

    int intSliderValue = 10;
    float floatSliderValue = 10.0f;

    ImGuiStyle& style1 = ImGui::GetStyle();

    // Image variables

    vector<Image> activeImages;
    int activeImageIndex = 0;
    bool filterApplied = false;


    bool imageDisplayed = true;
    bool loadingImage = false;

    bool textboxOpen = true;

    bool updateResultImage = false;
    bool updateOriginalImage = false;
    Image resultImage;

    Image originalImage;
    Image tempImage;

    int choice = 0;
    bool skewPopup = false;
    bool skewDegPopup = false;
    bool stateTrans = false;
    bool skewPending = false;

    string imageName;
    string inputImageName;


    int blurRadius = 0;
    float contrastValue = 0.0f;
    float skewDeg = 0.0f;
    float brightnessValue = 100;

    int pixelSize = 1;

    int motionBlurDirection = 0, motionBlurRadius = 1;
    bool motionBlurPopup = false, motionBlurPending = false;

    int rotateAngle = 0;
    bool rotatePopup = false;
    bool rotatePending = false;



    int nextFramePopup = 0;

    int frameType = 0;
    bool frameTypePopup = false;

    int frameChoice = 0;
    float thicknessPercentage = 0.0f;
    float linesPercentage = 0.0f;
    int spacingPercentage = 0;
    bool thicknessPopup = false;
    bool framePopup = false;
    bool framePending = false;
    bool triangularBorderPending = false;

    int choiceColor = 0;
    int choiceColor2 = 0;
    bool colorPopup = false;
    bool colorPopup2 = false;
    bool colorPending = false;

    ImVec4 Color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 Color2 = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    int rSlider = 0, gSlider = 0, bSlider = 0;
    int rSlider2 = 0, gSlider2 = 0, bSlider2 = 0;
    bool rgbPopup = false;
    bool rgbPopup2 = false;
    bool rgbPending = false;

    int cropPosX = 0, cropPosY = 0;
    int cropWidth = 0, cropHeight = 0;
    bool cropPopup = false;
    bool cropPending = false;

    int newWidth = 0, newHeight = 0;
    bool resizePopup = false;
    bool resizePending = false;

    Image mergeImage;
    string mergeName;
    int popupOrder = 0;
    bool mergePopup = false;
    bool sizeMismatchPopup1 = false;
    bool sizeChoicePopup1 = false;
    bool sizeMismatchPopup2 = false;
    bool sizeChoicePopup2 = false;
    bool sizeMismatchPopup3 = false;
    bool sizeChoicePopup3 = false;
    bool sizeMismatchPopup4 = false;
    bool sizeChoicePopup4 = false;
    bool mergePending = false;
    int mergeChoice1 = 0;
    int mergeChoice2 = 0;
    int mergeChoice3 = 0;
    int mergeChoice4 = 0;

    float swirlStrength = 0;
    bool swirlPopup = false;
    bool swirlPending = false;
    bool goldenRatioChosen = false;

    int ampx = 0, ampy = 0, frequency = 0;
    bool vertical = false;
    bool horizontal = false;
    bool both = false;
    bool wavyPopup = false;
    bool wavyPending = false;

    const    double pi = 3.14159265358979323846;

    bool showText = false;



    static bool selecting = false;
    static ImVec2 selectStart, selectEnd;

    //handle mouse input


    int imageCorX = 0;
    int imageCorY = 0;
    int imageCorW = 0;
    int imageCorH = 0;

    ImVec2 buttonSize = ImVec2(150, 45);

    float dataWindowWidth = 0.0f;
    float dataWindowHeight = 0.0f;
    ImVec2 dataWindowSize = ImVec2(dataWindowWidth, dataWindowHeight);

    // Main loop
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        ImVec2 imagePos = ImGui::GetCursorScreenPos();
        ImVec2 imageSize = ImVec2(resultImage.width, resultImage.height);

        //image centering
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        float displayWidth = displaySize.x;
        float displayHeight = displaySize.y;
        float displayRatio = displayWidth / displayHeight;

        float originalImageWidth = (float)originalImage.width;
        float originalImageHeight = (float)originalImage.height;
        float originalImageSize = originalImageWidth * originalImageHeight;
        float originalImageRatio = originalImageWidth / originalImageHeight;

        float resultImageWidth = (float)resultImage.width;
        float resultImageHeight = (float)resultImage.height;
        float resultImageSize = resultImageWidth * resultImageHeight;
        float resultImageRatio = resultImageWidth / resultImageHeight;

        float maxWidth = displayWidth * 0.3f;
        float maxHeight = displayHeight * 0.3f;

        float scale = min(maxWidth / originalImageWidth, maxHeight / originalImageHeight);  //to keep ascpect ratio

        float drawWidth = originalImageWidth * scale;
        float drawHeight = originalImageHeight * scale;

        float maxW = displayWidth * 0.4f;
        float maxH = displayHeight * 0.4f;

        float scaleOrigin = min(maxW / originalImage.width, maxH / originalImage.height);

        originalImageWidth = originalImage.width * scaleOrigin;
        originalImageHeight = originalImage.height * scaleOrigin;

        float scaleResult = min(maxW / resultImage.width, maxH / resultImage.height);
        resultImageWidth = resultImage.width * scaleResult;
        resultImageHeight = resultImage.height * scaleResult;

        if (open)
        {
            //preferrences
            ImGui::GetStyle().FrameRounding = 12.0f;
            ImGuiStyle& style = ImGui::GetStyle();

            ImGuiIO& io = ImGui::GetIO();


            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
            style.WindowBorderSize = 0.0f;

            ImGui::GetIO().FontGlobalScale = 1.0f; // 150%



            ImVec2 pos = ImGui::GetMainViewport()->Pos;
            ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + 30));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x - dataWindowSize.x, ImGui::GetMainViewport()->Size.y - 20));
            if (ImGui::Begin("Pixplode", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar))
            {
                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("Load image"))
                        {
                            imageName = OpenFileDialog();
                            if (!imageName.empty())
                                loadingImage = true;
                        }
                        if (ImGui::MenuItem("Save image"))
                        {
                            string saveName = OpenSaveFileDialog();
                            if (!saveName.empty())
                            {
                                resultImage.saveImage(saveName);
                            }
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMainMenuBar();
                }
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2) - (ImGui::CalcTextSize("Pixplode").x / 2));
                ImGui::Text("Pixplode");

                if (loadingImage)
                {
                    LoadImage(tempImage, imageName);
                    originalImage = tempImage; //Storing the original image
                    resultImage = tempImage; //Storing the result image
                    loadingImage = false;
                    updateOriginalImage = true;
                    updateResultImage = true;
                    activeImages.clear();
                    activeImages.push_back(resultImage);
                    activeImageIndex = 0;
                    showText = true;
                }
                if (updateOriginalImage)
                {
                    UpdateImage(originalImage); //updates GPU texture
                    updateOriginalImage = false;
                }
                float textWidth = ImGui::CalcTextSize("Original Image").x;
                float imgWidth = (float)originalImage.width;
                ImGui::BeginGroup(); // Lock X position
                ImGui::Image((void*)(intptr_t)originalImage.srv, ImVec2(originalImageWidth, originalImageHeight)); // Display original
                if (showText)
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (originalImageWidth - textWidth) * 0.5f); // to centter the text
                    ImGui::Text("Original Image");
                }
                ImGui::EndGroup();

                ImVec2 originalImgPos = ImGui::GetItemRectMin();
                ImVec2 originalImgSize = ImGui::GetItemRectSize();
                bool hoveredOriginal = ImGui::IsItemHovered();


                ImGui::SameLine();
                if (updateResultImage)
                {
                    UpdateImage(resultImage); //updates GPU texture
                    updateResultImage = false;
                }
                float resultTextWidth = ImGui::CalcTextSize("Result Image").x;
                float resultImgWidth = (float)resultImage.width;
                ImGui::BeginGroup(); // Lock X position
                ImGui::Image((void*)(intptr_t)resultImage.srv, ImVec2(resultImageWidth, resultImageHeight));
                if (showText)
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (resultImageWidth - textWidth) * 0.5f);
                    ImGui::Text("Result Image");
                }
                ImGui::EndGroup();

                ImVec2 resultImgPos = ImGui::GetItemRectMin();
                ImVec2 resultImgSize = ImGui::GetItemRectSize();
                bool hoveredResult = ImGui::IsItemHovered();


                ImVec2 imagePos = resultImgPos;
                ImVec2 imageSize = resultImgSize;

                ////drag with mouse to select area
                ImVec2 mousePos = ImGui::GetIO().MousePos;
                if (hoveredResult && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) //check if mouse is clicked
                {
                    selecting = true;
                    selectStart = mousePos;
                    selectEnd = mousePos;
                }
                if (selecting)
                {
                    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    {
                        selectEnd = mousePos; //update while mouse is down
                    }
                    else
                    {
                        selecting = false; //stop selecting when mouse is released
                        imageCorX = (int)(selectStart.x - imagePos.x);
                        imageCorY = (int)(selectStart.y - imagePos.y);
                        imageCorW = (int)(selectEnd.x - selectStart.x);
                        imageCorH = (int)(selectEnd.y - selectStart.y);

                        if (imageCorW < 0) //handle negative width/height
                        {
                            imageCorX += imageCorW;
                            imageCorW = -imageCorW;
                        }
                        if (imageCorH < 0)
                        {
                            imageCorY += imageCorH;
                            imageCorH = -imageCorH;
                        }

                        //clamp values
                        float scaleX = (float)resultImage.width / imageSize.x;
                        float scaleY = (float)resultImage.height / imageSize.y;

                        int x1 = (int)((selectStart.x - imagePos.x) * scaleX);
                        int y1 = (int)((selectStart.y - imagePos.y) * scaleY);
                        int x2 = (int)((selectEnd.x - imagePos.x) * scaleX);
                        int y2 = (int)((selectEnd.y - imagePos.y) * scaleY);

                        // get top-left corner and positive width/height
                        imageCorX = min(x1, x2);
                        imageCorY = min(y1, y2);
                        imageCorW = abs(x2 - x1);
                        imageCorH = abs(y2 - y1);

                        // clamp to image bounds
                        imageCorX = max(imageCorX, min(0, resultImage.width));
                        imageCorY = max(imageCorY, min(0, resultImage.height));
                        imageCorW = min(imageCorW, resultImage.width - imageCorX);
                        imageCorH = min(imageCorH, resultImage.height - imageCorY);
                    }
                }
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                if (hoveredResult && selecting)
                {
                    drawList->AddRectFilled(selectStart, selectEnd, IM_COL32(255, 255, 0, 50));
                    drawList->AddRect(selectStart, selectEnd, IM_COL32(255, 255, 0, 200));
                }

                if (filterApplied)
                {
                    activeImages.resize(activeImageIndex + 1); // truncate any redo history
                    activeImages.push_back(resultImage);     // push the new version of my originall image
                    activeImageIndex++;
                    filterApplied = false;
                }
                if (ImGui::Button("Undo", ImVec2(65, 30)))
                {
                    if (activeImageIndex > 0)
                    {
                        activeImageIndex--;
                        tempImage =
                            activeImages[activeImageIndex];
                        resultImage = tempImage;
                        updateResultImage = true;
                    }
                }
                ImVec2 undoButtonPos = ImGui::GetItemRectMin();             //to set the table position relative to undo button
                ImVec2 undoButtonSize = ImGui::GetItemRectSize();
                ImGui::SameLine();
                if (ImGui::Button("Redo", ImVec2(65, 30)))
                {
                    if (activeImageIndex < activeImages.size() - 1)
                    {
                        activeImageIndex++;
                        tempImage = activeImages[activeImageIndex];
                        resultImage = tempImage;
                        updateResultImage = true;
                    }
                }
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f)); // hover
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.1f, 0.1f, 1.0f)); // pressed

                if (ImGui::Button("Reset", ImVec2(65, 30)))
                {
                    resultImage = originalImage;
                    tempImage = resultImage;
                    updateResultImage = true;
                }
                ImGui::PopStyleColor(3);
                //making a table for buttons
                float tableWidth = (buttonSize.x + ImGui::GetStyle().ItemSpacing.x) * 2 + ImGui::GetStyle().WindowPadding.x * 2;
                float availWidth = ImGui::GetContentRegionAvail().x; // Get available width

                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(25, 25)); // Add padding between columns
                ImGui::SetCursorPos(ImVec2(undoButtonPos.x, undoButtonPos.y + 30)); // Center the table
                if (ImGui::BeginTable("lll", 5, ImGuiTableFlags_None))
                {
                    //first column
                    ImGui::TableNextColumn();
                    if (ImGui::Button("Gray Scale", buttonSize))
                    {
                        GrayScale(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Black And White", buttonSize))
                    {
                        BlackAndWhite(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Invert", buttonSize))
                    {
                        invert(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Retro Tv", buttonSize))
                    {
                        RetroTV(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Infrared", buttonSize))
                    {
                        infrared(tempImage);
                        resultImage = tempImage;
                        filterApplied = true;
                        updateResultImage = true;
                    }
                    if (ImGui::Button("Natural sunlight", buttonSize))
                    {
                        AdjustWarmth(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }

                    //second column
                    ImGui::TableNextColumn();
                    if (ImGui::Button("Purple filter", buttonSize))
                    {
                        purple(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Oil paint", buttonSize))
                    {
                        OilPainting(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Edge detection", buttonSize))
                    {
                        EdgeDetect(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Flip horizontally", buttonSize))
                    {
                        FlipImageHorizontally(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Flip vertically", buttonSize))
                    {
                        FlipImageVertically(tempImage);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    if (ImGui::Button("Skew image", buttonSize))
                    {
                        skewDegPopup = true;
                        ImGui::OpenPopup("Skew Deg");
                    }

                    ImVec2 popupSize = ImVec2(600, 600);
                    ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) / 2, (ImGui::GetIO().DisplaySize.y - 300) / 2), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(popupSize, ImGuiCond_Always);
                    if (ImGui::BeginPopupModal("Skew Deg", &skewDegPopup))
                    {
                        ImGui::Text("Set skew degree (0, 90)");
                        ImGui::SliderFloat("Skew angle", &skewDeg, 0, 90);
                        if (ImGui::Button("Apply", buttonSize))
                        {
                            stateTrans = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    if (stateTrans)
                    {
                        skewPopup = true;
                        ImGui::OpenPopup("Skew direction");
                        stateTrans = false;
                    }
                    if (ImGui::BeginPopupModal("Skew direction", &skewPopup))
                    {
                        ImGui::Text("Choose one");
                        if (ImGui::Button("1 - Skew left", buttonSize))
                        {
                            choice = 1;
                            skewPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2 - Skew right", buttonSize))
                        {
                            choice = 2;
                            skewPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    if (skewPending)
                    {
                        skew(tempImage, skewDeg, choice);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;

                        // Reset flags after applying
                        skewPending = false;
                        skewPopup = false;
                    }
                    //third column
                    ImGui::TableNextColumn();
                    if (ImGui::Button("Rotate", buttonSize))
                    {
                        rotatePopup = true;
                        ImGui::OpenPopup("Rotate angle");
                    }
                    ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) / 2, (ImGui::GetIO().DisplaySize.y - 300) / 2), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(popupSize, ImGuiCond_Always);
                    if (ImGui::BeginPopupModal("Rotate angle", &rotatePopup))
                    {
                        ImGui::Text("Choose one");
                        if (ImGui::Button("1 - 90", buttonSize))
                        {
                            rotateAngle = 1;
                            rotatePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2 - 180", buttonSize))
                        {
                            rotateAngle = 2;
                            rotatePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("3 - 270", buttonSize))
                        {
                            rotateAngle = 3;
                            rotatePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    if (rotatePending)
                    {
                        rotate(tempImage, rotateAngle);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                        // Reset flags after applying
                        rotatePending = false;
                        rotatePopup = false;
                    }
                    if (ImGui::Button("Crop image", buttonSize))
                    {
                        CropImage(tempImage, imageCorX, imageCorY, imageCorW, imageCorH);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                        //// Reset flags after applying
                        //cropPending = false;
                        //cropPopup = false;
                    }
                    if (ImGui::Button("Resize image", buttonSize))
                    {
                        resizePopup = true;
                        ImGui::OpenPopup("Resize options");
                    }
                    if (ImGui::BeginPopupModal("Resize options", &resizePopup))
                    {

                        ImGui::Text("Set new image size");
                        ImGui::InputInt("Image width", &newWidth);
                        ImGui::InputInt("Image height", &newHeight);
                        if (ImGui::Button("Apply", buttonSize))
                        {
                            resizePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    if (resizePending)
                    {
                        Resize(tempImage, newWidth, newHeight);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                        // Reset flags after applying
                        resizePending = false;
                        resizePopup = false;
                    }
                    //merge
                    if (ImGui::Button("Merge with image", buttonSize))
                    {
                        mergeName = OpenFileDialog();
                        if (!mergeName.empty())
                        {
                            LoadImage(mergeImage, mergeName);
                            mergePopup = true;
                        }
                    }

                    if (mergePopup)
                    {
                        int mergeSize = mergeImage.width * mergeImage.height;
                        int tempSize = tempImage.width * tempImage.height;

                        if (mergeSize == tempSize)
                        {
                            mergePending = true;
                            mergePopup = false;
                        }
                        else if (mergeImage.width < tempImage.width || mergeImage.height < tempImage.height)
                        {
                            popupOrder = 1;
                            mergePopup = false;
                        }
                        else if (mergeImage.width > tempImage.width || mergeImage.height > tempImage.height)
                        {
                            popupOrder = 3;
                            mergePopup = false;
                        }
                    }

                    if (ImGui::BeginPopupModal("Different Dimensions", &sizeMismatchPopup1))
                    {
                        ImGui::Text("The image you chose is smaller than the first image");
                        if (ImGui::Button("1- Resize"))
                        {
                            mergeChoice1 = 1;
                            popupOrder = 2;
                            sizeMismatchPopup1 = false;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2- Merge the common area"))
                        {
                            mergeChoice1 = 2;
                            mergePending = true;
                            sizeMismatchPopup1 = false;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }

                    if (ImGui::BeginPopupModal("Choose which image to fit the other:", &sizeMismatchPopup2))
                    {
                        if (ImGui::Button("1- First image fits second image"))
                        {
                            mergeChoice2 = 1;
                            mergePending = true;
                            sizeMismatchPopup2 = false;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2- Second image fits first image"))
                        {
                            mergeChoice2 = 2;
                            mergePending = true;
                            sizeMismatchPopup2 = false;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }

                    if (ImGui::BeginPopupModal("Different Image Dimensions", &sizeMismatchPopup3))
                    {
                        ImGui::Text("The image you chose is bigger than the first image");
                        if (ImGui::Button("1- Resize"))
                        {
                            mergeChoice3 = 1;
                            popupOrder = 4;
                            sizeMismatchPopup3 = false;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2- Crop the new image"))
                        {
                            mergeChoice3 = 2;
                            mergePending = true;
                            sizeMismatchPopup3 = false;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }

                    if (ImGui::BeginPopupModal("Choose which to fit the other", &sizeMismatchPopup4))
                    {
                        if (ImGui::Button("1- First image fits second image"))
                        {
                            mergeChoice4 = 1;
                            mergePending = true;
                            sizeMismatchPopup4 = false;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2- Second image fits first image"))
                        {
                            mergeChoice4 = 2;
                            mergePending = true;
                            sizeMismatchPopup4 = false;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }

                    if (popupOrder == 1)
                    {
                        sizeMismatchPopup1 = true;
                        ImGui::OpenPopup("Different Dimensions");
                        popupOrder = 0;
                    }
                    else if (popupOrder == 2)
                    {
                        sizeMismatchPopup2 = true;
                        ImGui::OpenPopup("Choose which image to fit the other:");
                        popupOrder = 0;
                    }
                    else if (popupOrder == 3)
                    {
                        sizeMismatchPopup3 = true;
                        ImGui::OpenPopup("Different Image Dimensions");
                        popupOrder = 0;
                    }
                    else if (popupOrder == 4)
                    {
                        sizeMismatchPopup4 = true;
                        ImGui::OpenPopup("Choose which image to fit the other");
                        popupOrder = 0;
                    }

                    if (mergePending)
                    {
                        merge(tempImage, mergeImage, mergeChoice1, mergeChoice2, mergeChoice3, mergeChoice4);

                        resultImage =
                            tempImage;
                        updateResultImage = true;
                        filterApplied = true;

                        mergePending = false;
                        mergePopup = false;
                        sizeMismatchPopup1 = false;
                        sizeMismatchPopup2 = false;
                        sizeMismatchPopup3 = false;
                        sizeMismatchPopup4 = false;
                        mergeChoice1 = mergeChoice2 = mergeChoice3 = mergeChoice4 = 0;
                        popupOrder = 0;
                    }
                    if (ImGui::Button("Make a frame", buttonSize))
                    {
                        frameTypePopup = true;
                        ImGui::OpenPopup("Frame type");
                    }
                    ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) / 2, (ImGui::GetIO().DisplaySize.y - 300) / 2), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(popupSize, ImGuiCond_Always);
                    if (ImGui::BeginPopupModal("Frame type", &frameTypePopup))
                    {
                        ImGui::Text("Choose one");
                        if (ImGui::Button("1 - Border frame", buttonSize))
                        {
                            frameChoice = 1;
                            nextFramePopup = 1;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2 - Simple lines frame", buttonSize))
                        {
                            frameChoice = 2;
                            nextFramePopup = 1;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("3 - Triangular Border"))
                        {
                            frameChoice = 3;
                            nextFramePopup = 1;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    /*ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) / 2, (ImGui::GetIO().DisplaySize.y - 300) / 2), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(popupSize, ImGuiCond_Always);
                    if (ImGui::BeginPopupModal("Frame options", &framePopup))
                    {
                        ImGui::Text("Choose one");
                        if (ImGui::Button("1 - Simple black", buttonSize))
                        {
                            frameChoice = 1;
                            nextFramePopup = 1;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2 - Simple white", buttonSize))
                        {
                            frameChoice = 2;
                            nextFramePopup = 1;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }*/
                    ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) / 2, (ImGui::GetIO().DisplaySize.y - 300) / 2), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(popupSize, ImGuiCond_Always);
                    if (ImGui::BeginPopupModal("Frame thickness", &thicknessPopup))
                    {
                        ImGui::Text("Set frame thickness percentage (0-20%)");
                        ImGui::SliderFloat("Border percentage", &thicknessPercentage, 0.0f, 20.0f);
                        ImGui::SliderFloat("Lines thickness", &linesPercentage, 0.0f, 20.0f);
                        ImGui::SliderInt("Spacing", &spacingPercentage, 0, 20);

                        if (ImGui::Button("Apply", buttonSize))
                        {
                            nextFramePopup = 2;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    if (ImGui::BeginPopupModal("Color choice", &colorPopup))
                    {
                        ImGui::Text("Choose color");
                        if (ImGui::Button("1 - Red", buttonSize))
                        {
                            choiceColor = 1;
                            if (frameChoice == 3)
                            {
                                nextFramePopup = 5;
                            }
                            else
                                framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2 - Blue", buttonSize))
                        {
                            choiceColor = 2;
                            if (frameChoice == 3)
                            {
                                nextFramePopup = 5;
                            }
                            else
                                framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("3 - Green", buttonSize))
                        {
                            choiceColor = 3;
                            if (frameChoice == 3)
                            {
                                nextFramePopup = 5;
                            }
                            else
                                framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("4 - Black", buttonSize))
                        {
                            choiceColor = 4;
                            if (frameChoice == 3)
                            {
                                nextFramePopup = 5;
                            }
                            else
                                framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("5 - White", buttonSize))
                        {
                            choiceColor = 5;
                            if (frameChoice == 3)
                            {
                                nextFramePopup = 5;
                            }
                            else
                                framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("6 - Custom RGB", buttonSize))
                        {
                            choiceColor = 6;
                            nextFramePopup = 3;
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
                    }
                    if (ImGui::BeginPopupModal("Second Color Choice", &colorPopup2))
                    {
                        ImGui::Text("Choose second color");
                        if (ImGui::Button("1 - Red", buttonSize))
                        {
                            choiceColor2 = 1;
                            framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2 - Blue", buttonSize))
                        {
                            choiceColor2 = 2;
                            framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("3 - Green", buttonSize))
                        {
                            choiceColor2 = 3;
                            framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("4 - Black", buttonSize))
                        {
                            choiceColor2 = 4;
                            framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("5 - White", buttonSize))
                        {
                            choiceColor2 = 5;
                            framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("6 - Custom RGB", buttonSize))
                        {
                            choiceColor2 = 6;
                            nextFramePopup = 3;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) / 2, (ImGui::GetIO().DisplaySize.y - 300) / 2), ImGuiCond_Always);
                    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Always);
                    if (ImGui::BeginPopupModal("Custom RGB values", &rgbPopup, ImGuiWindowFlags_NoResize))
                    {
                        ImGui::Text("Set RGB values");
                        ImGui::ColorPicker4("Color picker", (float*)&Color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                        /*ImGui::SliderInt("R", &rSlider, 0, 255);
                        ImGui::SliderInt("G", &gSlider, 0, 255);
                        ImGui::SliderInt("B", &bSlider, 0, 255);*/
                        if (ImGui::Button("Apply", buttonSize))
                        {
                            if (frameChoice == 3)
                            {
                                nextFramePopup = 5;
                            }
                            else
                            {
                                framePending = true;
                            }
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    int R = (int)(Color.x * 255);
                    int G = (int)(Color.y * 255);
                    int B = (int)(Color.z * 255);
                    int A = (int)(Color.w * 255);





                    if (ImGui::BeginPopupModal("Custom RGB values for second color", &rgbPopup2, ImGuiWindowFlags_NoResize))
                    {
                        ImGui::Text("Set RGB values (0-255)");
                        ImGui::ColorPicker4("Color picker", (float*)&Color2, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                        /*ImGui::SliderInt("R", &rSlider2, 0, 255);
                        ImGui::SliderInt("G", &gSlider2, 0, 255);
                        ImGui::SliderInt("B", &bSlider2, 0, 255);*/
                        if (ImGui::Button("Apply", buttonSize))
                        {
                            framePending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    int R2 = (int)(Color2.x * 255);
                    int G2 = (int)(Color2.y * 255);
                    int B2 = (int)(Color2.z * 255);
                    int A2 = (int)(Color2.w * 255);

                    //Popup managing
                    if (nextFramePopup == 1)
                    {
                        thicknessPopup = true;
                        ImGui::OpenPopup("Frame thickness");
                        nextFramePopup = 0;
                    }
                    else if (nextFramePopup == 2)
                    {
                        colorPopup = true;
                        ImGui::OpenPopup("Color choice");
                        nextFramePopup = 0;
                    }
                    else if (nextFramePopup == 3)
                    {
                        rgbPopup = true;
                        ImGui::OpenPopup("Custom RGB values");
                        nextFramePopup = 0;
                    }
                    else if (nextFramePopup == 4)
                    {
                        framePopup = true;
                        ImGui::OpenPopup("Frame options");
                        nextFramePopup = 0;
                    }
                    else if (nextFramePopup == 5)
                    {
                        colorPopup2 = true;
                        ImGui::OpenPopup("Second Color Choice");
                        nextFramePopup = 0;
                    }
                    else if (nextFramePopup == 6)
                    {
                        rgbPopup2 = true;
                        ImGui::OpenPopup("Custom RGB values for second color");
                        nextFramePopup = 0;
                    }
                    if (framePending)
                    {
                        frame(tempImage, frameChoice, thicknessPercentage, choiceColor, R, G, B, choiceColor2, R2, G2, B2, spacingPercentage, linesPercentage);
                        filterApplied = true;
                        resultImage = tempImage;
                        updateResultImage = true;
                        // Reset flags after applying
                        framePending = false;
                        framePopup = false;
                        colorPopup = false;
                        rgbPopup = false;
                    }
                    if (ImGui::Button("Swirl", buttonSize))
                    {
                        swirlPopup = true;
                        ImGui::OpenPopup("Swirl Strength");
                    }

                    if (ImGui::BeginPopupModal("Swirl Strength", &swirlPopup))
                    {
                        ImGui::Text("use swirl factor:");
                        if (ImGui::Button("Swirl Factor", buttonSize))
                        {
                            swirlPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::SliderFloat("Swirl Strength", &swirlStrength, 0, 50);
                        ImGui::Text("Or use Golden Ratio");
                        if (ImGui::Button("Use Golden Ratio", buttonSize))
                        {
                            goldenRatioChosen = true;
                            swirlPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }

                    if (swirlPending)
                    {
                        Swirl(tempImage, swirlStrength, goldenRatioChosen);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                        swirlPending = false;
                        swirlPopup = false;
                    }


                    //fourth column
                    ImGui::TableNextColumn();
                    if (ImGui::Button("Pixelize", buttonSize))
                    {
                        pixelize(tempImage, pixelSize);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    ImGui::SliderInt("Pixel size", &pixelSize, 1, 100);

                    if (ImGui::Button("Adjust Contrast", buttonSize))
                    {
                        AdjustContrast(tempImage, contrastValue);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    ImGui::SliderFloat("Contrast factor", &contrastValue, 0.0f, 2.5f);


                    if (ImGui::Button("Adjust brightness", buttonSize))
                    {
                        float factor = (brightnessValue / 100);
                        AdjustBrightness(tempImage, factor);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    ImGui::SliderFloat("Brightness factor", &brightnessValue, 0, 200);


                    if (ImGui::Button("Wavy Effect", buttonSize))
                    {
                        wavyPopup = true;
                        ImGui::OpenPopup("Wavy options");
                    }
                    if (ImGui::BeginPopupModal("Wavy options", &wavyPopup))
                    {
                        ImGui::Text("Choose one");
                        if (ImGui::Button("1 - Vertical waves", buttonSize))
                        {
                            vertical = true;
                            horizontal = false;
                            both = false;
                            wavyPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2 - Horizontal waves", buttonSize))
                        {
                            vertical = false;
                            horizontal = true;
                            both = false;
                            wavyPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("3 - Both directions", buttonSize))
                        {
                            vertical = false;
                            horizontal = false;
                            both = true;
                            wavyPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    if (wavyPending)
                    {
                        Wavy(tempImage, ampx, ampy, frequency, vertical, horizontal, both);
                        resultImage =
                            tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                        wavyPending = false;
                        wavyPopup = false;
                    }

                    ImGui::SliderInt("Vertical Amplitude", &ampx, 0, 30);
                    ImGui::SliderInt("Horizontal Amplitude", &ampy, 0, 30);
                    ImGui::SliderInt("Frequency", &frequency, 1, 30);



                    //fifth column
                    ImGui::TableNextColumn();
                    if (ImGui::Button("Blur", buttonSize))
                    {
                        blur(tempImage, blurRadius);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                    }
                    ImGui::SliderInt("Blur radius", &blurRadius, 0, 100);

                    if (ImGui::Button("Motion Blur", buttonSize))
                    {
                        motionBlurPopup = true;
                        ImGui::OpenPopup("Motion Blur direction");
                    }
                    if (ImGui::BeginPopupModal("Motion Blur direction"))
                    {
                        if (ImGui::Button("1- Horizontal", buttonSize))
                        {
                            motionBlurDirection = 1;
                            motionBlurPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("2- Vertical", buttonSize))
                        {
                            motionBlurDirection = 2;
                            motionBlurPending = true;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    if (motionBlurPending)
                    {
                        motionblur(tempImage, motionBlurDirection, motionBlurRadius);
                        resultImage = tempImage;
                        updateResultImage = true;
                        filterApplied = true;
                        motionBlurPending = false;
                        motionBlurPopup = false;
                    }
                    ImGui::SliderInt("Motion Blur radius", &motionBlurRadius, 1, 100);







                    ImGui::PopStyleVar();
                    ImGui::EndTable();
                }




                /*if(ImGui::Button("Crop image"))
                {
                    cropPopup = true;
                    ImGui::OpenPopup("Crop options");
                }*/
                /*ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) / 2, (ImGui::GetIO().DisplaySize.y - 300) / 2), ImGuiCond_Always);
                ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);
                if(ImGui::BeginPopupModal("Crop options", &cropPopup))
                {
                    ImGui::Text("Set crop position and size");
                    ImGui::SliderInt("X position", &cropPosX, 0, originalImage.width);
                    ImGui::SliderInt("Y position", &cropPosY, 0, originalImage.height);
                    ImGui::SliderInt("Crop width", &cropWidth, cropPosX+1, originalImage.width-cropPosX);
                    ImGui::SliderInt("Crop height", &cropHeight, cropPosY+1, originalImage.height-cropPosY);
                    if (ImGui::Button("Apply", ImVec2(120, 0)))
                    {
                        cropPending = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }*/






            }ImGui::End();
            ImGui::SameLine();
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 300, pos.y + 30), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetMainViewport()->Size.y - 20), ImGuiCond_Always);
            if (ImGui::Begin("Image Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::SetWindowFontScale(1.4f);
                ImGui::Text("Original Image:");
                ImGui::Spacing();
                ImGui::Text("Width: %d px", originalImage.width);
                ImGui::Text("Height: %d px", originalImage.height);
                ImGui::Text("Channels: %d", originalImage.channels);
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Text("Result Image:");
                ImGui::Spacing();
                ImGui::Text("Width: %d px", resultImage.width);
                ImGui::Text("Height: %d px", resultImage.height);
                ImGui::Text("Channels: %d", resultImage.channels);
                ImGui::SetWindowFontScale(1.0f);

                dataWindowSize = ImGui::GetWindowSize();
                dataWindowHeight = ImGui::GetWindowHeight();
                dataWindowWidth = ImGui::GetWindowWidth();
            }ImGui::End();
        }


        // Rendering
        ImGui::Render();
        ImVec4 clear_color = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}
void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }

}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
