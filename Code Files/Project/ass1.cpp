// Created by Mahmoud Ehab 20240549, Mohamed Ahmed Hamza 20242265, Yomna Abbas 20242399
// All of us are in Section 6.
// Mahmoud Ehab did Grayscale, Merge, Brightness adjustment, Edge detection, Skew, Infrared,  participated in Main Menu and Load/Save image
// Mohamed Ahmed Hamza did BlackAndWhite, Crop Image, Flip Image Horizontally and Vertically, Resize, Natural light, Oil painting and Main Menu and Load/Save Image
// Yomna Abbas did Invert Image, Rotate Image, Add Frame, Blur Image, Retro TV, Purple Filter
// Repo Link: https://github.com/Mohamed-ElwanH/OOP-Assignment-1
// Shared Document Link: https://docs.google.com/document/d/1GlZuzqQaO4qLRR6GKcV0H3zPLlk1UjhFW2EkxvNyn-o/edit?tab=t.0
// Youtube Video Showcasing: https://youtu.be/fyok_bIvHCw
// The File applies different filters on images based on the user's choice.

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

#include "ass1.h"
using namespace std;
void LoadCheck(string userInput, Image& usedImage)
{
    try
    {
        usedImage.loadNewImage(userInput);
        cout << "Image loaded successfully" << endl;
        return;
    }
    catch (

        const invalid_argument& error)
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

        const invalid_argument& x)
    {
        cin >> imageName;
        SaveCheck(usedImage, imageName);
    }
}

void safeCopy(const Image& src, Image& dst)
{
    // Allocate destination correctly
    dst.width = src.width;
    dst.height = src.height;
    dst.channels = src.channels;

    // Allocate a new data buffer (free old one if needed)
    if (dst.imageData)
    {
        delete[] dst.imageData;
        dst.imageData = nullptr;
    }

    size_t dataSize = static_cast<size_t>(src.width) * src.height * src.channels;
    dst.imageData = new unsigned char[dataSize];

    // Copy pixel data safely
    std::memcpy(dst.imageData, src.imageData, dataSize);
}

void GrayScale(Image& image) // Converts the image to greyscale
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
void BlackAndWhite(Image& image) // Converts the image to black and white
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
void invert(Image& image) // Inverts the colors of the image
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
void rotate(Image& image, int deg) // Rotates the image by 90, 180 or 270 degrees depending on the user's choice
{
    if (deg == 1) // Rotate 90 degrees
    {
        Image rotated(image.height, image.width);
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
        Image rotated(image.width, image.height);
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
        Image rotated(image.height, image.width);
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
void FlipImageHorizontally(Image& image) // Flips the image horizontally
{
    int x = 0;
    Image emptyImage(image.width, image.height); // creating an empty image
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
void FlipImageVertically(Image& image) // Flips the image vertically
{
    int y = image.height - 1;
    Image emptyImage(image.width, image.height); // Creates an empty image
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
void CropImage(Image& image, int x, int y, int width, int height) // Crops the image based on the user's input
{
    int emptyX = 0;
    Image emptyImage(width, height);
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

void AdjustBrightness(Image& image, float factor) // increases or decreases the brightness based on the user's input (1.5 for 50% increase and 0.5 for 50% decrease)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                int value = static_cast<int>(image(i, j, k) * factor);

                image(i, j, k) = min(255, max(0, value));
            }
        }
    }
}

void blur(Image& image, int blurRadius)
{

    if (blurRadius < 1) return;

    // horizontal run
    for (int j = 0; j < image.height; j++)
    {
        for (int k = 0; k < 3; k++)
        {                          // loops over one color in all rows
            std::vector<int> row(image.width); // store blurred row temporarily
            int sum = 0, cntr = 0; // sum of the pixels in the window and counter for valid pixels

            for (int i = 0; i < image.width && i < blurRadius; i++)
            { // fills the first window
                sum += image(i, j, k);
                cntr++;
            }

            for (int i = 0; i < image.width; i++)
            {
                int avg = (cntr > 0) ? round(sum / cntr) : 0; // assigns the average to the blurred image
                if (avg > 255) avg = 255;
                if (avg < 0) avg = 0;
                row[i] = avg; // store result in row buffer

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

            // copy blurred row back to the image
            for (int i = 0; i < image.width; i++)
                image(i, j, k) = row[i];
        }
    }

    // vertical run
    for (int i = 0; i < image.width; i++)
    {
        for (int k = 0; k < 3; k++)
        { // loops over one color in all columns
            std::vector<int> col(image.height); // store blurred column temporarily
            int sum = 0, cntr = 0;

            for (int j = 0; j < image.height && j < blurRadius; j++)
            { // fills the first window
                sum += image(i, j, k);
                cntr++;
            }

            for (int j = 0; j < image.height; j++)
            {
                int avg = (cntr > 0) ? round(sum / cntr) : 0;
                if (avg > 255) avg = 255;
                if (avg < 0) avg = 0;
                col[j] = avg; // store blurred column value

                if (j - blurRadius >= 0)
                {
                    sum -= image(i, j - blurRadius, k);
                    cntr--;
                } // removes the old pixel
                if (j + blurRadius + 1 < image.height)
                {
                    sum += image(i, j + blurRadius + 1, k);
                    cntr++;
                } // adds the new pixel
            }

            // copy blurred column back to image
            for (int j = 0; j < image.height; j++)
                image(i, j, k) = col[j];
        }
    }
}

void getRGB(unsigned char frameColor[3], int r, int g, int b)
{
    frameColor[0] = static_cast<unsigned char>(r);
    frameColor[1] = static_cast<unsigned char>(g);
    frameColor[2] = static_cast<unsigned char>(b);
}
void getColorchoice(unsigned char frameColor[3], int choiceColor, int r, int g, int b)
{
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
        getRGB(frameColor, r, g, b);
        break;
    default:
        break;
    }
}

void frame(Image& image, int frameChoice, float thicknessPerct, int colorChoice, int r, int g, int b, int colorChoice2, int r2, int g2, int b2, int space, float thick)
{
    if (frameChoice == 1)
    {
        thicknessPerct /= 100;

        int border = thicknessPerct * min(image.height, image.width);

        int frameWidth = image.width + 2 * border;
        int frameHeight = image.height + 2 * border;
        Image frame(frameWidth, frameHeight);

        unsigned char frameColor[3] = { 0, 0, 0 }; // rgb value control
        getColorchoice(frameColor, colorChoice, r, g, b);

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

    else if (frameChoice == 2)
    {
        unsigned char frameColor[3] = { 0, 0, 0 }; // rgb value control
        int offset = 0.05 * min(image.height, image.width);
        int lineThickness = offset / 3;
        int gap = lineThickness / 2; // gap between lines

        getColorchoice(frameColor, colorChoice, r, g, b);

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
    else if (frameChoice == 3)
    {
        thicknessPerct /= 100;
        int border = thicknessPerct * min(image.height, image.width);
        int frameWidth = image.width + 2 * border;
        int frameHeight = image.height + 2 * border;
        Image frame(frameWidth, frameHeight);

        unsigned char frameColor[3] = { 0, 0, 0 };
        getColorchoice(frameColor, colorChoice, r, g, b);

        for (int i = 0; i < frame.width; i++)
            for (int j = 0; j < frame.height; j++)
                for (int k = 0; k < 3; k++)
                    frame(i, j, k) = frameColor[k];
        int base = min(image.width, image.height);

        int spacing = max(10, base / space);
        int thickness = spacing / thick;

        unsigned char lineColor[3] = { 0, 0, 0 };
        getColorchoice(lineColor, colorChoice2, r2, g2, b2);

        for (int i = 0; i < frame.width; i++)
            for (int j = 0; j < frame.height; j++)
            {
                bool drawLine = false;
                int mod1 = ((i - j) % (2 * spacing) + (2 * spacing)) % (2 * spacing);
                int mod2 = ((i + j) % (2 * spacing) + (2 * spacing)) % (2 * spacing);

                if (min(mod1, 2 * spacing - mod1) < thickness)
                    drawLine = true;
                if (min(mod2, 2 * spacing - mod2) < thickness)
                    drawLine = true;

                if (drawLine)
                    for (int k = 0; k < 3; k++)
                        frame(i, j, k) = lineColor[k];
            }

        for (int i = 0; i < image.width; i++)
            for (int j = 0; j < image.height; j++)
                for (int k = 0; k < 3; k++)
                    frame(i + border, j + border, k) = image(i, j, k);

        image = frame;
    }
}
//void triangularBorder(Image& image, float percentage, int choiceColor, int r1, int g1, int b1, int r2, int g2, int b2)
//{
//    /*cout << "Enter the thickness of the border (1-40): ";
//    cin >> percentage;*/
//    percentage /= 100;
//
//    int border = percentage * min(image.height, image.width);
//    int frameWidth = image.width + 2 * border;
//    int frameHeight = image.height + 2 * border;
//    Image frame(frameWidth, frameHeight);
//
//    unsigned char frameColor[3] = { 0, 0, 0 };
//    getColorchoice(frameColor, choiceColor, r1, g1, b1);
//
//    for (int i = 0; i < frame.width; i++)
//        for (int j = 0; j < frame.height; j++)
//            for (int k = 0; k < 3; k++)
//                frame(i, j, k) = frameColor[k];
//    int base = min(image.width, image.height);
//
//    int spacing = max(10, base / 50);
//    int thickness = spacing / 3;
//
//    unsigned char lineColor[3] = { 0, 0, 0 };
//    getColorchoice(lineColor, r2, g2, b2);
//
//    for (int i = 0; i < frame.width; i++)
//        for (int j = 0; j < frame.height; j++)
//        {
//            bool drawLine = false;
//            int mod1 = ((i - j) % (2 * spacing) + (2 * spacing)) % (2 * spacing);
//            int mod2 = ((i + j) % (2 * spacing) + (2 * spacing)) % (2 * spacing);
//
//            if (min(mod1, 2 * spacing - mod1) < thickness)
//                drawLine = true;
//            if (min(mod2, 2 * spacing - mod2) < thickness)
//                drawLine = true;
//
//            if (drawLine)
//                for (int k = 0; k < 3; k++)
//                    frame(i, j, k) = lineColor[k];
//        }
//
//    for (int i = 0; i < image.width; i++)
//        for (int j = 0; j < image.height; j++)
//            for (int k = 0; k < 3; k++)
//                frame(i + border, j + border, k) = image(i, j, k);
//
//    image = frame;
//}

void Resize

(Image& image, int newWidth, int newHeight)
{
    Image newImage(newWidth, newHeight);
    float wRatio = static_cast<float>(image.width) / newWidth;
    float hRatio = static_cast<float>(image.height) / newHeight;
    for (int i = 0; i < newImage.width; i++)
    {
        for (int j = 0; j < newImage.height; j++)
        {
            int x= min(static_cast<int>(roundf(i * wRatio)), image.width - 1);
            int y = min(static_cast<int>(roundf(j * hRatio)), image.height - 1);
            for (int k = 0; k < 3; k++)
            {
                newImage(i, j, k) = image(x, y, k);
            }
        }
    }
    image = newImage;
}





//void merge(Image& image, Image& mergeMe, int mergeChoice1, int mergeChoice2, int mergeChoice3, int mergeChoice4) // Merges two images by averaging their pixel values
//{
//    //string mergename;
//    //cout << "Input the name of the image you want to merge" << endl;
//    //cin >> mergename;
//    //LoadCheck(mergename, mergeMe);
//
//    bool crop = false;
//    //int choice;
//    //if (image.width > mergeMe.width || image.height > mergeMe.height)
//
//        //cout << "The image you chose is smaller than the original image\n Choose whether to \n 1- Resize \n 2-Crop the original image\n";
//        //cin >> choice;
//    if (mergeChoice1 == 2)
//    {
//
//        crop = true;
//        CropImage(image, 0, 0, mergeMe.width, mergeMe.height);
//        image.width = mergeMe.width;
//        image.height = mergeMe.height;
//    }
//    if (mergeChoice1 == 1)
//    {
//        /*cout << "Choose whether to make the first image fit the second image or vice versa \n 1- First image fits second image \n 2- Second image fits first image \n";
//        cin >> mergeChoice2;*/
//        if (mergeChoice2 == 1)
//            Resize(image, mergeMe.width, mergeMe.height);
//        if (mergeChoice2 == 2)
//
//        {
//
//            Resize(mergeMe, image.width, image.height);
//        }
//    }
//
//
//
//    /*cout << "The image you chose is bigger than the original image\n Choose whether to \n 1- Resize \n 2-Crop the new image\n";
//    cin >> choice;*/
//    if (mergeChoice3 == 2)
//    {
//
//        crop = true;
//        CropImage(mergeMe, 0, 0, image.width, image.height);
//        image.width = mergeMe.width;
//        image.height = mergeMe.height;
//    }
//    if (mergeChoice3 == 1)
//    {
//        /*cout << "Choose whether to make the first image fit the second image or vice versa \n 1- First image fits second image \n 2- Second image fits first image \n";
//        cin >> choice;*/
//        if (mergeChoice4 == 1)
//            Resize(image, mergeMe.width, mergeMe.height);
//        if (mergeChoice4 == 2)
//
//        {
//
//            Resize(mergeMe, image.width, image.height);
//        }
//    }
//    Image mergedImage(image.width, image.height);
//    for (int i = 0; i < image.width; i++)
//    {
//        for (int j = 0; j < image.height; j++)
//        {
//            for (int k = 0; k < 3; k++)
//            {
//                mergedImage(i, j, k) = (image(i, j, k) + mergeMe(i, j, k)) / 2;
//            }
//        }
//    }
//    image = mergedImage;
//}




void merge(Image& image, Image& mergeMe,
    int mergeChoice1, int mergeChoice2,
    int mergeChoice3, int mergeChoice4)
{
    bool crop = false;

    // smaller second image case
    if (mergeChoice1 == 2)
    {
        crop = true;
        CropImage(image, 0, 0, mergeMe.width, mergeMe.height);
    }
    if (mergeChoice1 == 1)
    {
        if (mergeChoice2 == 1)
            Resize(image, mergeMe.width, mergeMe.height);
        if (mergeChoice2 == 2)
            Resize(mergeMe, image.width, image.height);
    }

    // bigger second image case
    if (mergeChoice3 == 2)
    {
        crop = true;
        CropImage(mergeMe, 0, 0, image.width, image.height);
    }
    if (mergeChoice3 == 1)
    {
        if (mergeChoice4 == 1)
            Resize(image, mergeMe.width, mergeMe.height);
        if (mergeChoice4 == 2)
            Resize(mergeMe, image.width, image.height);
    }

    // Now both have same width/height
    Image mergedImage(image.width, image.height);
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

    image = mergedImage; // safe if operator= frees memory
}


void EdgeDetect(Image& image)
{
    Image copy(image.width, image.height);
    copy = image;
    GrayScale(image);
    int Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1} };
    int Gy[3][3] =
    {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1} };
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
            int magnitude = min(255, max(0, static_cast<int>(sqrt(sumx * sumx + sumy * sumy))));

            for (int k = 0; k < 3; k++)
            {
                image(i, j, k) = 255 - magnitude;
            }
        }
    }
}

void RetroTV(Image& image)
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
                    int value = static_cast<int>(image(i, j, k) * 1.5);

                    image(i, j, k) = min(255, max(0, value));
                }
            }
        }
    }
}

void infrared(Image& image) // Converts the image to infrared colors
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
void skew(Image& image, float skewdeg, int choice)
{
    bool left = false;




    int newI = 0;
    skewdeg = skewdeg * (3.14159265358979323846 / 180.0); // Convert degrees to radians

    int newWidth = image.width + abs(image.height * tan(skewdeg));

    if (choice == 1)
    {
        left = true;
    }
    Image skewedImage(newWidth, image.height);
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (left)
                {

                    newI = (i + (abs(tan(skewdeg)) * (image.height - 1 - j)));
                    skewedImage(newI, j, k) = image(i, j, k);
                }
                else
                {
                    newI = (i + (tan(skewdeg) * j));
                    skewedImage(newI, j, k) = image(i, j, k);
                }
            }
        }
    }

    safeCopy(skewedImage, image);
}
void AdjustWarmth(Image& image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            image(i, j, 1) = (min<int>(255, max<int>(0, static_cast<int>(image(i, j, 1) * 1.12))));
            image(i, j, 2) = (min<int>(255, max<int>(0, static_cast<int>(image(i, j, 2) * 0.7))));
        }
    }
}
void OilPainting(Image& src)
{
    Image image = src;
    int intensityLevels = 30, radius = 1;
    for (int i = radius; i < image.width - radius; i++)
    {
        for (int j = radius; j < image.height - radius; j++)
        {
            vector<int> intensityCount(intensityLevels);
            vector<int> averageR(intensityLevels);
            vector<int> averageG(intensityLevels);
            vector<int> averageB(intensityLevels);
            for (int x = i - radius; x <= i + radius; x++)
            {
                for (int y = j - radius; y <= j + radius; y++) // each pixel, within radius 5 of pixel
                {
                    // For each sub-pixel, calculate the intensity, and determine which intensity that intensity number falls into.

                    int R = image(x, y, 0), G = image(x, y, 1), B = image(x, y, 2);
                    double avg = (R + G + B) / 3.0;
                    int curIntensity = static_cast<int>((avg * (intensityLevels - 1)) / 255.0);
                    curIntensity = max(0, min(curIntensity, intensityLevels - 1));
                    intensityCount[curIntensity]++;
                    averageR[curIntensity] += R;
                    averageG[curIntensity] += G;
                    averageB[curIntensity] += B;
                }
            }
            auto max_itrator = max_element(intensityCount.begin(), intensityCount.end());
            int maxIndex = distance(intensityCount.begin(), max_itrator);
            if (intensityCount[maxIndex] > 0) // apply the most common intensity average to the pixel
            {
                image(i, j, 0) = min(255, max(0, averageR[maxIndex] / intensityCount[maxIndex]));
                image(i, j, 1) = min(255, max(0, averageG[maxIndex] / intensityCount[maxIndex]));
                image(i, j, 2) = min(255, max(0, averageB[maxIndex] / intensityCount[maxIndex]));
            }
        }
    }
    src = image;
}
void purple(Image& image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            image(i, j, 0) = min(255.0, (image(i, j, 0) * 1.5));
            image(i, j, 1) = max(0.0, (image(i, j, 1) * 0.5));
            image(i, j, 2) = min(255.0, (image(i, j, 2) * 1.5));
        }
    }

}
void pixelize(Image& image, int pixelSize)
{
    for (int i = 0; i < image.width; i += pixelSize)
    {
        for (int j = 0; j < image.height; j += pixelSize)
        {
            int sum[3] = { 0, 0, 0 };
            int count = 0;

            for (int x = i; x < min(i + pixelSize, image.width); x++)
            {
                for (int y = j; y < min(j + pixelSize, image.height); y++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        sum[k] += image(x, y, k);
                    }
                    count++;
                }
            }

            for (int k = 0; k < 3; k++)
            {
                sum[k] /= count;
            }

            for (int x = i; x < min(i + pixelSize, image.width); x++)
            {
                for (int y = j; y < min(j + pixelSize, image.height); y++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        image(x, y, k) = sum[k];
                    }
                }
            }
        }
    }
}
void motionblur(Image& image, int direction, int blurRadius)
{
    if (direction == 1) { // horizontal blur
        Image blurredh(image.width, image.height), blurredv(image.width, image.height);

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
        image = blurredh;
    }
    else if (direction == 2) { // vertical blur
        Image blurredh(image.width, image.height);
        for (int i = 0; i < image.width; i++)
        {

            for (int k = 0; k < 3; k++)
            { // loops over one color in all columns
                int sum = 0, cntr = 0;
                for (int j = 0; j < image.height && j < blurRadius; j++)
                { // fills the first window
                    sum += image(i, j, k);
                    cntr++;
                }
                for (int j = 0; j < image.height; j++)
                {
                    int avg = round(sum / cntr);
                    if (avg > 255)
                        avg = 255;
                    if (avg < 0)
                        avg = 0;
                    blurredh(i, j, k) = avg;
                    if (j - blurRadius >= 0)
                    {
                        sum -= image(i, j - blurRadius, k);
                        cntr--;
                    } // removes the old pixel
                    if (j + blurRadius + 1 < image.height)
                    {
                        sum += image(i, j + blurRadius + 1, k);
                        cntr++;
                    } // adds the new pixel
                }
            }
        }

        image = blurredh;
    }
}
void AdjustContrast(Image& image, float contrastFactor)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int R = image(i, j, 0), G = image(i, j, 1), B = image(i, j, 2);
            int R2 = ((R - 128) * contrastFactor) + 128;
            int G2 = ((G - 128) * contrastFactor) + 128;
            int B2 = ((B - 128) * contrastFactor) + 128;
            image(i, j, 0) = min(255, max(0, R2));
            image(i, j, 1) = min(255, max(0, G2));
            image(i, j, 2) = min(255, max(0, B2));
        }
    }
}
void Swirl(Image& image, float strength, bool goldenRatioChosen)
{
    int CenterX = image.width / 2;
    int CenterY = image.height / 2;
    Image swirled(image.width, image.height);
    int maxRadius = min(CenterX, CenterY);
    const    double pi = 3.14159265358979323846;
    double k = (2 * pi / 1.6180);
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            double distanceX = i - CenterX;
            double distanceY = j - CenterY;
            double r = sqrt(distanceX * distanceX + distanceY * distanceY);
            double theta = atan2(distanceY, distanceX);
            float newtheta = 0.0f;
            if (!goldenRatioChosen)
            {
                newtheta = theta + strength * (maxRadius - r) / maxRadius; // Swirl effect decreases with distance
            }
            else
            {
                newtheta = theta + k * log(r + 1); //Golden Ratio Swirl
            }
            int newX = CenterX + r * cos(newtheta);
            int newY = CenterY + r * sin(newtheta);
            if (newX >= 0 && newX < image.width - 1 && newY >= 0 && newY < image.height - 1)
            {
                int x1 = floor(newX);
                int y1 = floor(newY);
                int x2 = x1 + 1;
                int y2 = y1 + 1;

                float dx = newX - x1;
                float dy = newY - y1;

                for (int k = 0; k < 3; k++)
                {
                    float Q11 = image(x1, y1, k);
                    float Q21 = image(x2, y1, k);
                    float Q12 = image(x1, y2, k);
                    float Q22 = image(x2, y2, k);

                    float R1 = Q11 * (1 - dx) + Q21 * dx;
                    float R2 = Q12 * (1 - dx) + Q22 * dx;
                    float P = R1 * (1 - dy) + R2 * dy;

                    swirled(i, j, k) = (uint8_t)P;
                }
            }
            else
            {
                for (int k = 0; k < 3; k++)
                    swirled(i, j, k) = image(i, j, k);
            }
        }

    }
    image = swirled;
}
const    double pi = 3.14159265358979323846;
// In the Wavy function, initialize newX and newY at the start of each loop iteration
void Wavy(Image& image, int ampx, int  ampy, int frequency, bool vertical, bool horizontal, bool both) {
    Image effect(image.width, image.height);
    int newX = 0, newY = 0;
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            
            newX = i;
            newY = j;

            if (vertical)
            {
                newX = i + ampx * sin(2 * pi * frequency * j / image.height);
                newY = j;
            }
            if (horizontal)
            {
                newX = i;
                newY = j + ampy * sin(2 * pi * frequency * i / image.width);
            }
            if (both)
            {
                newX = i + ampx * sin(2 * pi * frequency * j / image.height);
                newY = j + ampy * sin(2 * pi * frequency * i / image.width);
            }
            if (newX >= 0 && newX < image.width - 1 && newY >= 0 && newY < image.height - 1)
            {
                int x1 = floor(newX);
                int y1 = floor(newY);
                int x2 = x1 + 1;
                int y2 = y1 + 1;

                float dx = newX - x1;
                float dy = newY - y1;

                for (int k = 0; k < 3; k++)
                {
                    float Q11 = image(x1, y1, k);
                    float Q21 = image(x2, y1, k);
                    float Q12 = image(x1, y2, k);
                    float Q22 = image(x2, y2, k);

                    float R1 = Q11 * (1 - dx) + Q21 * dx;
                    float R2 = Q12 * (1 - dx) + Q22 * dx;
                    float P = R1 * (1 - dy) + R2 * dy;

                    effect(i, j, k) = (uint8_t)P;
                }
            }
            else
            {
                for (int k = 0; k < 3; k++)
                    effect(i, j, k) = image(i, j, k);
            }
        }
    }
                        image = effect;
                    }


    //int GetChoice() // Displays the menu and gets the user's choice
    //{
    //    int choice;
    //    vector<string> choices =
    //    {
    //        "01- Load a new image",
    //        "02- GrayScale Filter",
    //        "03- Black And White Filter",
    //        "04- Invert Image",
    //        "05- Merge Two Images",
    //        "06- Flip Image",
    //        "07- Rotate Image",
    //        "08- Adjust Image Brightness",
    //        "09- Crop Image",
    //        "10- Add frame",
    //        "11- Detect Edges",
    //        "12- Resize Image",
    //        "13- Blur Image",
    //        "14- Natural Sunlight Filter",
    //        "15- Oil Painting Filter",
    //        "16- Retro Tv Filter",
    //        "17- Purple Filter",
    //        "18- Infrared Filter",
    //        "19- Skew Image",
    //        "20- Save Image",
    //        "21- Exit" };
    //    cout << "Choose" << endl;
    //    for (int i = 0; i < choices.size(); i++)
    //    {
    //        cout << choices[i] << endl;
    //    }
    //    cin >> choice;
    //    return choice;
    //}
    //int main() // main program
    //{
    //    string merged;
    //    bool menuDisplayed = true;
    //    Image usedImage;
    //    string userInput;
    //    cout << "Input image" << endl;
    //    cin >> userInput;
    //    LoadCheck(userInput, usedImage);
    //    int newWidth, newHeight;
    //    string imageName;
    //    while (menuDisplayed) // Menu loop until user chooses to exit
    //    {
    //        switch (GetChoice())
    //        {
    //        case 1: // Load new image
    //            cout << "Input image" << endl;
    //            cin >> userInput;
    //            LoadCheck(userInput, usedImage);
    //            break;
    //        case 2:
    //            GrayScale(usedImage);
    //            break;
    //        case 3:
    //            BlackAndWhite(usedImage);
    //            break;
    //        case 4:
    //            invert(usedImage);
    //            break;
    //        case 5:
    //            merge(usedImage);
    //            break;
    //        case 6: // Flip options
    //            int choice;
    //            cout << "1- Flip image horizontally" << endl
    //                << "2- Flip image vertically" << endl << "3- cancel" << endl;
    //            cin >> choice;
    //            switch (choice)
    //            {
    //            case 1:
    //                FlipImageHorizontally(usedImage);
    //                break;
    //            case 2:
    //                FlipImageVertically(usedImage);
    //                break;
    //            default:
    //                break;
    //            }
    //            break;
    //        case 7:
    //            rotate(usedImage);
    //            break;
    //        case 8:
    //            cout << "Enter brightness adjustment factor ";
    //            float factor;
    //            cin >> factor;
    //            cout << "Adjusting brightness by a factor of " << factor << endl;
    //            AdjustBrightness(usedImage, factor);
    //            break;
    //        case 9:
    //            int x, y, width, height;
    //            cout << "Provide the starting point of x axis and y axis" << endl;
    //            cin >> x >> y;
    //            cout << "Provide the width and height of the area you would like to crop" << endl;
    //            cin >> width >> height;
    //            CropImage(usedImage, x, y, width, height);
    //            break;
    //        case 10:
    //            frame(usedImage);
    //            break;
    //        case 11:
    //            EdgeDetect(usedImage);
    //            break;
    //        case 12:
    //            cout << "Enter the new width then the new height.";
    //            cin >> newWidth >> newHeight;
    //            Resize(usedImage, newWidth, newHeight);
    //            break;
    //        case 13:
    //            blur(usedImage);
    //
    //            if (usedImage.imageData == nullptr)
    //            {
    //                cout << "Error: Image data is empty after blur!" << endl;
    //            }
    //            cout << usedImage.width << " " << usedImage.height << endl;
    //            break;
    //        case 14:
    //            AdjustWarmth(usedImage);
    //            break;
    //        case 15:
    //            OilPainting(usedImage);
    //            break;
    //        case 16:
    //            RetroTV(usedImage);
    //            break;
    //        case 17:
    //            purple(usedImage);
    //            break;
    //        case 18:
    //            infrared(usedImage);
    //            break;
    //        case 19:
    //            skew(usedImage);
    //            break;
    //        case 20:
    //            cout << "Type the name that you would like to save the image with" << endl;
    //            cin >> imageName;
    //            SaveCheck(usedImage, imageName);
    //            break;
    //
    //        case 21:
    //            menuDisplayed = false;
    //            break;
    //
    //        default:
    //            break;
    //        }
    //    }
    //    return 0;
    //}
