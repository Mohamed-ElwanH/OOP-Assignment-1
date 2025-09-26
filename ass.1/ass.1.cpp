#include "image_class.h"
#include <bits/stdc++.h>
using namespace std;
void GrayScale(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++) // get avg of every 3 leds and equate the pixel to it
        {
            int avg = 0;
            for (int k = 0; k < 3; k++)
            {
                avg += image(i, j, k);
            }
            avg /= 3;
            for (int k = 0; k < 3; k++)
            {
                image(i, j, k) = avg;
            }
        }
    }
    image.saveImage("GrayScale.png");
}
void BlackAndWhite(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++) // get avg of every 3 leds and equate the pixel to it
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
    image.saveImage("BlackAndWhite.png");
}
void FlipImageHorizontally(Image &image)
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
    image.saveImage("FlipH.png");
}
void FlipImageVertically(Image &image)
{
    int y = image.height-1;
    Image emptyImage(image.width, image.height); // creating an empty image
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
    image.saveImage("FlipV.png");
}
int GetChoice()
{
    int choice;
    vector<string> choices = {"1-GrayScale filter", "2-BlackAndWhite filter", "3-Flip Image Horizontally", "4-Flip Image Vertically"};
    cout << "Choose" << endl;
    for (int i = 0; i < choices.size(); i++)
    {
        cout << choices[i] << endl;
    }
    cin >> choice;
    return choice;
}
int main()
{
    Image usedImage;
    string userInput;
    cout << "Input image" << endl;
    cin >> userInput;
    usedImage.loadNewImage(userInput);
    switch (GetChoice())
    {
    case 1:
        GrayScale(usedImage);
        break;
    case 2:
        BlackAndWhite(usedImage);
        break;
    case 3:
        FlipImageHorizontally(usedImage);
        break;
    case 4:
        FlipImageVertically(usedImage);
        break;
    default:
        break;
    }
}