// Created by Mahmoud Ehab 20240549,Mohamed Ahmed Hamza 20242265, Yomna Abbas 20242399
// Section X (Can't register yet)
// Mahmoud Ehab did GreyScale,Adjust Brightness and participated in Main Menu and Load/Save Image
// Mohamed Ahmed Hamza did BlackAndWhite, Crop Image, Flip Image Horizontally and Vertically and  Main Menu and Load/Save Image
// Yomna Abbas did Invert Image, Rotate Image
// Repo Link: https://github.com/Mohamed-ElwanH/OOP-Assignment-1
// The File applies different filters on images based on the user's choice.
#include "image_class.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;
void LoadCheck(string userInput, Image &usedImage)
{
    try
    {
        usedImage.loadNewImage(userInput);
        cout << "Image loaded successfully" << endl;
        return;
    }
    catch (

        const invalid_argument &error)
    {
        cout << error.what() << "\nTry again\n"
             << endl;
        cin >> userInput;
        LoadCheck(userInput, usedImage);
    }
}
void SaveCheck(Image usedImage, string imageName)
{
    try
    {
        // cin >> imageName;
        usedImage.saveImage(imageName);
        cout << "Saved Successfully" << endl;
        return;
    }
    catch (

        const invalid_argument &x)
    {
        cin >> imageName;
        SaveCheck(usedImage, imageName);
    }
}

void GrayScale(Image &image) // Converts the image to greyscale
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int avg = 0; // Gets the sum of the RGB values of each pixel
            for (int k = 0; k < 3; k++)
            {
                avg += image(i, j, k);
            }
            avg /= 3; // Gets the average then assigns it to the three channels
            for (int k = 0; k < 3; k++)
            {
                image(i, j, k) = avg;
            }
        }
    }
}
void BlackAndWhite(Image &image) // Converts the image to black and white
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int avg = 0;
            for (int k = 0; k < 3; k++)
            {
                avg += image(i, j, k);
            }
            avg /= 3;
            for (int k = 0; k < 3; k++)
            {
                if (avg <= 255 / 2)
                {
                    image(i, j, k) = 0;
                }
                else
                {
                    image(i, j, k) = 255;
                }
            }
        }
    }
}
void invert(Image &image) // Inverts the colors of the image
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                image(i, j, k) = 255 - image(i, j, k);
            }
        }
    }
}
void rotate(Image &image) // Rotates the image by 90, 180 or 270 degrees depending on the user's choice
{
    int deg;
    cout << "choose a rotation degree: \n 1-90 \n 2-180 \n 3-270 \n";
    cin >> deg;
    if (deg == 1) // Rotate 90 degrees
    {
        Image rotated

            (image.height, image.width);
        for (int i = 0; i < image.height; i++)
        {
            for (int j = 0; j < image.width; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    rotated(i, j, k) = image(j, image.height - 1 - i, k); // new row = old column
                }
            }
        }
        image = rotated;
    }
    else if (deg == 2) // Rotate 180 degrees
    {
        Image rotated

            (image.width, image.height);
        for (int i = 0; i < image.width; i++)
        {
            for (int j = 0; j < image.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    rotated(i, j, k) = image(image.width - 1 - i, image.height - 1 - j, k); // rows flipped vertically col flipped horizontally
                }
            }
        }
        image = rotated;
    }
    else if (deg == 3) // Rotate 270 degrees
    {
        Image rotated

            (image.height, image.width);
        for (int i = 0; i < image.height; i++)
        {
            for (int j = 0; j < image.width; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    rotated(i, j, k) = image(image.width - 1 - j, i, k); // new column = old row
                }
            }
        }

        image = rotated;
    }
}
void FlipImageHorizontally(Image &image) // Flips the image horizontally
{
    int x = 0;
    Image emptyImage

        (image.width, image.height); // creating an empty image
    for (int i = image.width - 1; i >= 0; i--)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                emptyImage(x, j, k) = image(i, j, k);
            }
        }
        x++;
    }
    image = emptyImage;
}
void FlipImageVertically(Image &image) // Flips the image vertically
{
    int y = image.height - 1;
    Image emptyImage

        (image.width, image.height); // Creates an empty image
    for (int j = 0; j < image.height; j++)
    {
        for (int i = 0; i < image.width; i++)
        {
            for (int k = 0; k < 3; k++)
            {
                emptyImage(i, y, k) = image(i, j, k);
            }
        }
        y--;
    }
    image = emptyImage;
}
void CropImage(Image &image, int x, int y, int width, int height) // Crops the image based on the user's input
{
    int emptyX = 0;
    Image emptyImage

        (width, height);
    for (int i = x; i < x + width; i++)
    {
        int emptyY = 0;
        for (int j = y; j < y + height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                emptyImage(emptyX, emptyY, k) = image(i, j, k);
            }
            emptyY++;
        }
        emptyX++;
    }
    image = emptyImage;
}

void AdjustBrightness(Image &image, float factor) // increases or decreases the brightness based on the user's input (1.5 for 50% increase and 0.5 for 50% decrease)
{

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                int value = static_cast<

                    int>(image(i, j, k) * factor);

                image(i, j, k) = min(255, max(0, value));
            }
        }
    }
}

void blur(Image &image)
{
    Image blurredh

        (image.width, image.height),
        blurredv(image.width, image.height);
    int blurRadius;
    cout << "Enter blur radius 0-100 \n";
    cin >> blurRadius;

    // horizontal run
    for (int j = 0; j < image.height; j++)
    {

        for (int k = 0; k < 3; k++)
        {                          // loops over one color in all rows
            int sum = 0, cntr = 0; // sum of the pixels in the window and counter for valid pixels
            for (int i = 0; i < image.width && i < blurRadius; i++)
            { // fills the first window
                sum += image(i, j, k);
                cntr++;
            }
            for (int i = 0; i < image.width; i++)
            {
                int avg = round(sum / cntr); // assigns the average to the blurred image
                if (avg > 255)
                    avg = 255;
                if (avg < 0)
                    avg = 0;
                blurredh(i, j, k) = avg;

                if (i - blurRadius >= 0)
                {
                    sum -= image(i - blurRadius, j, k);
                    cntr--;
                } // removes the old pixel
                if (i + blurRadius + 1 < image.width)
                {
                    sum += image(i + blurRadius + 1, j, k);
                    cntr++;
                }; // adds the new pixel
            }
        }
    }
    // vertical run

    for (int i = 0; i < image.width; i++)
    {

        for (int k = 0; k < 3; k++)
        { // loops over one color in all columns
            int sum = 0, cntr = 0;
            for (int j = 0; j < image.height && j < blurRadius; j++)
            { // fills the first window
                sum += blurredh(i, j, k);
                cntr++;
            }
            for (int j = 0; j < image.height; j++)
            {
                int avg = round(sum / cntr);
                if (avg > 255)
                    avg = 255;
                if (avg < 0)
                    avg = 0;
                blurredv(i, j, k) = avg;
                if (j - blurRadius >= 0)
                {
                    sum -= blurredh(i, j - blurRadius, k);
                    cntr--;
                } // removes the old pixel
                if (j + blurRadius + 1 < image.height)
                {
                    sum += blurredh(i, j + blurRadius + 1, k);
                    cntr++;
                } // adds the new pixel
            }
        }
    }

    image = blurredv;
}

void getRGB(unsigned char frameColor[3])
{
    cout << "Enter RGB values from 0-255 consecutively \n";

    for (int i = 0; i < 3; i++)
    {
        int inpt;
        cin >> inpt;
        if (inpt < 0 || inpt > 255)
        {
            cout << "Invalid input, try again: ";
            i--; // Retry this input
            continue;
        }
        frameColor[i] = static_cast<

            unsigned char>(inpt);
    }
}
void getColorchoice(unsigned char frameColor[3])
{
    int choiceColor;
    cout << "enter a color \n 1-Red \n 2- Blue \n 3-Green \n 4-Black \n 5-White \n 6-Custom color \n";
    cin >> choiceColor;
    switch (choiceColor)
    {
    case 1:
        frameColor[0] = 255;
        frameColor[1] = 0;
        frameColor[2] = 0;
        break;
    case 2:
        frameColor[0] = 0;
        frameColor[1] = 0;
        frameColor[2] = 255;
        break;
    case 3:
        frameColor[0] = 0;
        frameColor[1] = 255;
        frameColor[2] = 0;
        break;
    case 4:
        frameColor[0] = 0;
        frameColor[1] = 0;
        frameColor[2] = 0;
        break;
    case 5:
        frameColor[0] = 255;
        frameColor[1] = 255;
        frameColor[2] = 255;
        break;
    case 6:
        getRGB(frameColor);
        break;
    default:
        cout << "invalid input, try again \n";
        getRGB(frameColor);
        break;
    }
}

void frame(Image &image)
{
    int choice;
    cout << "1-border \n2-simple lines frame \n";
    cin >> choice;

    if (choice == 1)
    {
        float percentage;
        cout << "enter the thickness of the border 1-40 \n";
        cin >> percentage;
        percentage /= 100;

        int border = percentage * min(image.height, image.width);

        int frameWidth = image.width + 2 * border;
        int frameHeight = image.height + 2 * border;
        Image frame

            (frameWidth, frameHeight);

        unsigned char

            frameColor[3] = {
                0, 0, 0}; // rgb value control
        getColorchoice(frameColor);

        for (int i = 0; i < frame.width; i++) // fill the border with the chosen color
        {
            for (int j = 0; j < frame.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    frame(i, j, k) = frameColor[k];
                }
            }
        }

        int lineThickness = border / 2;

        for (int i = 0; i < image.width; i++) // place the image in the middle of the frame
        {
            for (int j = 0; j < image.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {

                    frame(i + border, j + border, k) = image(i, j, k);
                }
            }
        }

        image = frame;
    }

    else if (choice == 2)
    {
        unsigned char

            frameColor[3] = {
                0, 0, 0}; // rgb value control
        int offset = 0.05 * min(image.height, image.width);
        int lineThickness = offset / 3;
        int gap = lineThickness / 2; // gap between lines

        getColorchoice(frameColor);

        for (int i = 0; i < image.width; i++) // place the image in the middle of the frame
        {
            for (int j = 0; j < image.height; j++)
            {

                bool isFramePixel = false;

                if (
                    // Left vertical lines
                    (i > offset && i < offset + lineThickness) ||
                    (i > offset + lineThickness + gap && i < offset + gap + 2 * lineThickness) ||
                    // Right vertical lines
                    (i > image.width - offset - lineThickness && i < image.width - offset) ||
                    (i > image.width - offset - 2 * lineThickness - gap && i < image.width - offset - lineThickness - gap) ||
                    // Top horizontal lines
                    (j > offset && j < offset + lineThickness) ||
                    (j > offset + lineThickness + gap && j < offset + gap + 2 * lineThickness) ||
                    // Bottom horizontal lines
                    (j > image.height - offset - lineThickness && j < image.height - offset) ||
                    (j > image.height - offset - 2 * lineThickness - gap && j < image.height - offset - lineThickness - gap))
                {
                    isFramePixel = true;
                }
                if (isFramePixel)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        image(i, j, k) = frameColor[k];
                    }
                }
            }
        }
    }
}
void merge

    (Image &image) // Merges two images by averaging their pixel values
{
    Image mergeMe;
    string mergename;
    cout << "Input the name of the image you want to merge" << endl;
    cin >> mergename;
    LoadCheck(mergename, mergeMe);

    if (image.width > mergeMe.width || image.height > mergeMe.height)
    {
        CropImage(image, 0, 0, mergeMe.width, mergeMe.height);
        image.width = mergeMe.width;
        image.height = mergeMe.height;
    }
    else if (mergeMe.width > image.width || mergeMe.height > image.height)
    {
        CropImage(mergeMe, 0, 0, image.width, image.height);
    }
    Image mergedImage

        (image.width, image.height);
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                mergedImage(i, j, k) = (image(i, j, k) + mergeMe(i, j, k)) / 2;
            }
        }
    }
    image = mergedImage;
}
void EdgeDetect

    (Image &image)
{
    Image copy

        (image.width, image.height);
    copy = image;
    GrayScale(image);
    int Gx[3][3] =
        {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}};
    int Gy[3][3] =
        {
            {1, 2, 1},
            {0, 0, 0},
            {-1, -2, -1}};
    for (int i = 1; i < image.width - 1; i++)
    {
        for (int j = 1; j < image.height - 1; j++)
        {
            int sumx = 0;
            int sumy = 0;
            for (int m = -1; m <= 1; m++)
            {
                for (int n = -1; n <= 1; n++)
                {
                    sumx += copy(i + m, j + n, 0) * Gx[m + 1][n + 1];
                    sumy += copy(i + m, j + n, 0) * Gy[m + 1][n + 1];
                }
            }
            int magnitude = min(255, max(0, static_cast<

                                                int>(sqrt(sumx * sumx + sumy * sumy))));
            for (int k = 0; k < 3; k++)
            {
                image(i, j, k) = 255 - magnitude;
            }
        }
    }
}

void Resize

    (Image &image,

     int newWidth, int newHeight)
{
    Image newImage

        (newWidth, newHeight);
    float wRatio = static_cast<

                       float>(image.width) /
                   newWidth;
    float hRatio = static_cast<

                       float>(image.height) /
                   newHeight;
    for (int i = 0; i < newImage.width; i++)
    {
        for (int j = 0; j < newImage.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                newImage(i, j, k) = image(roundf(i * wRatio), roundf(j * hRatio), k);
            }
        }
    }
    image = newImage;
}
void RetroTV

    (Image &image)
{

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {

                if (j % 3 == 0)
                    image(i, j, k) *= 0.5;
                else
                {
                    int value = static_cast<

                        int>(image(i, j, k) * 1.5);

                    image(i, j, k) = min(255, max(0, value));
                }
            }
        }
    }
}

void infrared

    (Image &image) // Converts the image to infrared colors
{

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                image(i, j, 0) = 0;
            }
        }
    }

    invert(image);
}
void skew

    (Image &image)
{
    int choice;
    bool left = false;
    // cout << "1- skew left \n2- skew right \n";
    // cin >> choice;

    double skewdeg;
    cout << "Enter skew degree (in degrees): ";
    cin >> skewdeg;
    int newI = 0;
    skewdeg = skewdeg * (3.14159265358979323846 / 180.0); // Convert degrees to radians

    int newWidth = image.width + abs(image.height * tan(skewdeg));

    // if (choice == 1)
    //{
    //     left = true;
    // }
    Image skewedImage(newWidth, image.height);
    for (int i = 0; i < image.width - 1; i++)
    {
        for (int j = 0; j < image.height - 1; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                // if (left)
                //{

                //  newI = (i + (abs(tan(skewdeg)) * (image.height - 1 - j)));
                // skewedImage(newI, j, k) = image(i, j, k);
                //}
                // else
                //  {
                newI = (i + (tan(skewdeg) * j));
                skewedImage(newI, j, k) = image(i, j, k);
                //}
            }
        }
        image = skewedImage;
    }
}
void AdjustWarmth

    (Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            image(i, j, 0) = (min<

                              int>(255, max<

                                            int>(0, static_cast<

                                                        int>(image(i, j, 0) * 1.1))));
            image(i, j, 1) = (min<

                              int>(255, max<

                                            int>(0, static_cast<

                                                        int>(image(i, j, 1) * 1.2))));
            image(i, j, 2) = (min<

                              int>(255, max<

                                            int>(0, static_cast<

                                                        int>(image(i, j, 2)))));
        }
    }
}
int GetChoice

    () // Displays the menu and gets the user's choice
{
    int choice;
    vector<string> choices =
        {
            "1- Load a new image",
            "2- GrayScale Filter",
            "3- Black And White Filter",
            "4- Invert Image",
            "5- Merge Two Images",
            "6- Flip Image",
            "7- Rotate Image",
            "8- Adjust Image Brightness",
            "9- Crop Image",
            "10- Add frame",
            "11- Detect Edges",
            "12- Resize Image",
            "13- Blur Image",
            "14- Natural Sunlight Filter",
            "15- Retro Tv Filter",
            "16- Infrared Filter",
            "17-Skew Image",
            "18- Save Image",
            "19- Exit"};
    cout << "Choose" << endl;
    for (int i = 0; i < choices.size(); i++)
    {
        cout << choices[i] << endl;
    }
    cin >> choice;
    return choice;
}
int main

    () // main program
{
    string merged;
    bool menuDisplayed = true;
    Image usedImage;
    string userInput;
    cout << "Input image" << endl;
    cin >> userInput;
    LoadCheck(userInput, usedImage);
    int newWidth, newHeight;
    string imageName;
    while (menuDisplayed) // Menu loop until user chooses to exit
    {
        switch (GetChoice())
        {
        case 1: // Load new image
            cout << "Input image" << endl;
            cin >> userInput;
            LoadCheck(userInput, usedImage);
            break;
        case 2:
            GrayScale(usedImage);
            break;
        case 3:
            BlackAndWhite(usedImage);
            break;
        case 4:
            invert(usedImage);
            break;
        case 5:
            merge(usedImage);
            break;
        case 6: // Flip options
            int choice;
            cout << "1- Flip image horizontally" << endl
                 << "2- Flip image vertically" << "3- cancel" << endl;
            cin >> choice;
            switch (choice)
            {
            case 1:
                FlipImageHorizontally(usedImage);
                break;
            case 2:
                FlipImageVertically(usedImage);
                break;
            default:
                break;
            }
            break;
        case 7:
            rotate(usedImage);
            break;
        case 8:
            cout << "Enter brightness adjustment factor ";
            float factor;
            cin >> factor;
            cout << "Adjusting brightness by a factor of " << factor << endl;
            AdjustBrightness(usedImage, factor);
            break;
        case 9:
            int x, y, width, height;
            cout << "Provide the starting point of x axis and y axis" << endl;
            cin >> x >> y;
            cout << "Provide the width and height of the area you would like to crop" << endl;
            cin >> width >> height;
            CropImage(usedImage, x, y, width, height);
            break;
        case 10:
            frame(usedImage);
            break;
        case 11:
            EdgeDetect(usedImage);
            break;
        case 12:
            cin >> newWidth >> newHeight;
            Resize(usedImage, newWidth, newHeight);
            break;
        case 13:
            blur(usedImage);

            if (usedImage.imageData == nullptr)
            {
                cout << "Error: Image data is empty after blur!" << endl;
            }
            cout << usedImage.width << " " << usedImage.height << endl;
            break;
        case 14:
            AdjustWarmth(usedImage);
            break;
        case 15:
            RetroTV(usedImage);
            break;
        case 16:
            infrared(usedImage);
            break;
        case 17:
            skew(usedImage);
            break;
        case 18:
            cout << "Type the name that you would like to save the image with" << endl;
            cin >> imageName;
            SaveCheck(usedImage, imageName);
            break;

        case 19:
            menuDisplayed = false;
            break;
        default:
            break;
        }
    }
    return 0;
}
