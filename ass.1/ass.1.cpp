#include "image_class.h"
#include <bits/stdc++.h>
using namespace std;

void BlackAndWhite(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++) // get avg of every 3 pixels and equate them to it
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
int GetChoice()
{
    int choice;
    vector<string> choices = {"1-Black-And-White filter", "2-", "3-", "4-"};

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
    usedImage.loadNewImage("F:/programming/oop/ass.1/KedaElGenabTmam.png");
    switch (GetChoice())
    {
    case 1:
        BlackAndWhite(usedImage);
        usedImage.saveImage("NewImage.png");
        break;
    default:
        break;
    }
}


//main
//Ihab
