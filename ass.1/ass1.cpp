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
void invert (Image &image) {
    for (int i=0 ; i< image.width; i++){
        for(int j=0; j< image.height; j++){
            for (int k=0; k<3 ; k++) {
                image(i,j,k)= 255- image(i,j,k);

            }

        }
    }
}
void rotate (Image &image){
    
    int deg;
    cout<<"choose a rotation degree: \n 1-90 \n 2-180 \n 3-270 \n";
    cin>>deg;
    if (deg==1){
        Image rotated(image.height,image.width);
        for(int i=0;i<image.height;i++){
            for(int j=0;j<image.width;j++){
                for(int k=0;k<3;k++){
                    rotated(i,j,k)= image(j,image.height-1-i,k); //new row = old column
                }

            }
        }
        image=rotated;

    }
    else if(deg ==2 ){
        Image rotated(image.width,image.height);
        for (int i=0 ; i< image.width; i++){
            for(int j=0; j< image.height; j++){
                for(int k=0;k<3;k++){
                    rotated(i,j,k) = image(image.width-1-i,image.height-1-j,k); //rows flipped vertically col flipped horizontally
                
            }
            
        }
        
        
        }
        image = rotated;
    }
    else if(deg==3){
         Image rotated(image.height,image.width);
        for(int i=0;i<image.height;i++){
            for(int j=0;j<image.width;j++){
                for(int k=0;k<3;k++){
                    rotated(i,j,k)= image(image.width-1-j,i,k); //new column = old row
                }

            }
        }

        image=rotated;
    }
     
    
}
int GetChoice()
{
    int choice;
    vector<string> choices = {"1-GrayScale filter", "2-BlackAndWhite filter", "3-Invert filter", "4-rotate","5-"};
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
    case 3:
       invert(usedImage);
       usedImage.saveImage("NewInverted.png");
       break;
    case 4:
       rotate(usedImage);
       usedImage.saveImage("NewRotated.png");
       break;

    default:
        break;
    }
}

//y33
