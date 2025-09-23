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
}
int GetChoice()
{
    int choice;
    vector<string> choices = {"1-GrayScale filter", "2-", "3-", "4-"};
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
        usedImage.saveImage("NewImage.png");
        break;
    case 2:
        BlackAndWhite(usedImage);
        usedImage.saveImage("NewImage.png");
        break;
    default:
        break;
    }
}