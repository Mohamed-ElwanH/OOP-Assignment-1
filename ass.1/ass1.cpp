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
// void imgerror()
//{
//     while (!loadImage) // Loops until the image entered is valid
//     {
//         try
//     {
//             usedImage.loadNewImage(userInput);
//             loadImage = true;
//             cout << "Image loaded successfully" << endl;
//             break;
//         }
//         catch(

//        const exception& error)
//    {
//            cout << error.what() << "\nTry again\n" << endl;
//            cin >> userInput;
//        }
//    }
//}
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
}
void AdjustBrightness(Image &image, float factor) // increases or decreases the brightness based on the user's input (1.5 for 50% increase and 0.5 for 50% decrease)
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
void merge(Image &image, Image &mergeMe, Image &mergedImage) // Merges two images by averaging their pixel values
{

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
}
void Edging(Image &image)
{
    BlackAndWhite(image);
    int threshold;
    cin >> threshold;
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {

                if (i == 0 || j == 0 || i == image.width - 1 || j == image.height - 1)
                {
                    continue; // Skip border pixels
                }
                if (abs(image(i, j, k) - image(i + 1, j, k)) > threshold || abs(image(i, j, k) - image(i, j + 1, k)) > threshold)
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
void AdjustWarmth(Image &image, float warmthFactor)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int R = image(i, j, 0), G = image(i, j, 1), B = image(i, j, 2);
            int R2 = R + (R * warmthFactor);
            int G2 = G + (G * warmthFactor);
            // int B2 = B - (B * warmthFactor);
            image(i, j, 0) = min(255, max(0, R2));
            image(i, j, 1) = min(255, max(0, G2));
            // image(i, j, 2) = min(255, max(0, B2));
        }
    }
}
void AdjustContrast(Image &image, float contrastFactor)
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
void FixWano(Image &image, float warmthFactor, float contrastFactor)
{
    AdjustWarmth(image, warmthFactor);
    // AdjustContrast(image, contrastFactor);
}
void Resize(Image &image, int newWidth, int newHeight)
{
    Image newImage(newWidth, newHeight);
    float wRatio = static_cast<float>(image.width) / newWidth;
    float hRatio = static_cast<float>(image.height) / newHeight;
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

void LoadCheck(string userInput, Image &usedImage)
{
    try
    {
        usedImage.loadNewImage(userInput);
        cout << "Image loaded successfully" << endl;
        return;
    }
    catch (const invalid_argument &error)
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
    catch (const invalid_argument &x)
    {
        cin >> imageName;
        SaveCheck(usedImage, imageName);
    }
}
int GetChoice() // Displays the menu and gets the user's choice
{
    int choice;
    vector<string> choices = {
        "1- Load a new image",
        "2- GrayScale filter",
        "3- BlackAndWhite filter",
        "4- Flip image",
        "5- Brightness Adjustment",
        "6- Crop image",
        "7- Invert image",
        "8- Rotate image",
        "9- Edging",
        "10- Merge",
        "11- Save image",
        "12- Exit"};
    cout << "Choose" << endl;
    for (int i = 0; i < choices.size(); i++)
    {
        cout << choices[i] << endl;
    }
    cin >> choice;
    return choice;
}
int main() // main program
{
    string merged;
    bool menuDisplayed = true;
    bool loadImage = false;
    Image usedImage;
    string userInput;
    Image mergedImage(usedImage.width, usedImage.height);
    Image mergeMe(usedImage.width, usedImage.height);

    cout << "Input image" << endl;
    cin >> userInput;
    LoadCheck(userInput, usedImage);
    string imageName;
    bool wrongName = false;
    float warmthFactor;
    float contrastFactor;
    int newWidth, newHeight;

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
        case 4: // Flip options
            int choice;
            cout << "1- Flip image horizontally" << "2- Flip image vertically" << "3- cancel" << endl;
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
        case 5:
            cout << "Enter brightness adjustment factor ";
            float factor;
            cin >> factor;
            cout << "Adjusting brightness by a factor of " << factor << endl;
            AdjustBrightness(usedImage, factor);
            break;
        case 6:
            int x, y, width, height;
            cout << "Provide the starting point of x axis and y axis" << endl;
            cin >> x >> y;
            cout << "Provide the width and height of the area you would like to crop" << endl;
            cin >> width >> height;
            CropImage(usedImage, x, y, width, height);
            break;
        case 7:
            invert(usedImage);
            break;
        case 8:
            rotate(usedImage);
            break;
        case 9:
            Edging(usedImage);
            break;
        case 10:
            cout << "Input the image you want to merge" << endl;
            cin >> merged;

            merge(usedImage, mergeMe, mergedImage);
            usedImage = mergedImage;
            break;
        case 11:
            cout << "Type the name that you would like to save the image with" << endl;
            cin >> imageName;
            SaveCheck(usedImage, imageName);
            break;
        case 12:
            menuDisplayed = false;
            break;
        case 13:
            cin >> warmthFactor;
            cin >> contrastFactor;
            FixWano(usedImage, warmthFactor, contrastFactor);
        case 14:
            cin >> newWidth >> newHeight;
            Resize(usedImage, newWidth, newHeight);
            break;

        default:
            break;
        }
    }
    return 0;
}
