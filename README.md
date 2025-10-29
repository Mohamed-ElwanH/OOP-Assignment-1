# Image Filter Program

##  Overview
This is an **interactive desktop image editor written in C++, using Dear ImGui for the graphical interface and DirectX 11 for rendering.** that allows users to apply multiple filters and effects to images.  
It demonstrates practical image manipulation through pixel-level operations.

## Features
GrayScale Filter – Black and White Filter – Invert Image – Merge Two Images – Flip Image – Rotate Image – Adjust Image Brightness – Crop Image – Add Frame – Detect Edges – Resize Image – Blur Image – Natural Sunlight Filter – Retro TV Filter – Infrared Filter – Skew Image

## User Guide
1. Launch the Application
Run Pixplode.exe from the build directory or through Visual Studio after building the project. A window titled 'Pixplode' will appear with a clean, interactive interface.
2. Load an Image
From the top menu bar, click File → Load Image. A file dialog will open — select your image (supported formats: .png, .jpg, .jpeg, .bmp, .tga). The selected image will appear on the left panel as the Original Image and on the right as the Result Image.
3. Apply Filters and Transformations
Use the available buttons and sliders in the main interface to apply effects.
Examples:
- Gray Scale, Black & White, Invert, Retro TV, Infrared
- Flip, Rotate, Crop, Resize, Merge, Frame, Blur, Swirl, Pixelize, and many more.
Each button instantly updates the Result Image preview on the right side.
Adjust sliders (for brightness, contrast, blur radius, etc.) to fine-tune effects in real time.
4. Undo or Redo Changes
Use the Undo and Redo buttons above the filter panel to navigate through previous states. The application automatically keeps track of image versions as you edit.
5. Compare Original vs. Edited Image
The interface displays both images side-by-side for easy comparison. Image details (resolution, channels) appear in the Image Info panel on the right.
6. Save Your Edited Image
From the File → Save Image option, choose a file name and format for your edited image. Supported export formats: .png, .jpg, .bmp, .tga.
7. Exit the Application
When finished, simply close the window or press the standard X button. Your exported file will be saved in the chosen directory.

   
## Libraries Used
<iostream>
<string>
<vector>
<algorithm>
<d3d11.h>
<tchar.h>

"imgui.h"

"imgui_impl_win32.h"

"imgui_impl_dx11.h"

"ImGuiFileDialog.h"

"image_class.h"

"ass1.h"

## Team 
Mahmoud Ehab Ahmed 
ID : 20240549
Mohammed Ahmed Hamza
ID : 20242265
Yomna Abbas Sabry
ID : 20242399




