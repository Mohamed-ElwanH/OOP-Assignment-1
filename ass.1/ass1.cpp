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
    int y = image.height - 1;
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
void CropImage(Image &image, int x, int y, int width, int hight)
{
    int emptyX = 0;
    Image emptyImage(width, hight);
    for (int i = x; i < x + width; i++)
    { 
        int emptyY = 0;
        for (int j = y; j < y + hight; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                emptyImage(emptyX, emptyY, k) = image(i, j, k);
            }
            emptyY++;
        }
        emptyX++;
    }
    emptyImage.saveImage("crop.png");
}
void AdjustWarmth(Image image, int red, int green, int blue) //NOT READY, STILL  WORKING ON
{
    for(int i = 0; i < image.width; i++)
    {
        for(int j = 0; j < image.height; j++)
        {
                //for(int k = 3;k < 3;k++) //f(x)=αx+β
                {
                    image(i, j, 0) -= red;
                    image(i, j, 1) -= green;
                    image(i, j, 2) -= blue;
                } 
        }
    }
    image.saveImage("warmth.png");
}


/*The multiplication factor f = (source bitmap width) / (destination bitmap width).
In the picture above f = 8/3 = 2.6667
f > 1 means reduction, f < 1 means magnification

sx1 is the left postion of the red square, sx1 = f * x.
Similar, sy1 = f * y is the top position.
sx2 = sx1 + f.
sy2 = sy1 + f.

1. read source pixel[i,j]
2. extract the red, green and blue values
3. multiply these values by dx*dy and adding them up per color
4. repeat 1..3 for all overlapping pixels
5. pack the summed red, green and blue colors in a dword (32 bit integer)
6. store this dword in destination bitmap [x,y]
*/



// void brightness(Image image)
// {
//     bool inUse = true;
//     while (inUse)
//     {
//         float alphaScale;
//         int scale;
//         int response;
//         cout<<"1-Input increament scale\n"<<"2-Exit"<<endl;
//         cin>>response;
//         if(response == 1)
//         {
//             cin>>alphaScale;
//             //cin>>scale;
//             for(int i = 0; i < image.width; i++)
//             {
//             for(int j = 0; j < image.height; j++)
//             {
//                 float Lumi = 0.2126*image(i,j,0) + 0.7152*image(i,j,1) + 0.0722*image(i,j,2); //Get luminance
//                 image(i, j, 0) = image(i, j, 0) + (image(i, j, 0) * (alphaScale-1));
//                 image(i, j, 1) = image(i, j, 1) + (image(i, j, 1) * (alphaScale-1));
//                 image(i, j, 2) = image(i, j, 2) + (image(i, j, 2) * (alphaScale-1));
//             }
//             } 
//         }
//         else
//         {
//             inUse = false;
//         }
//     }
//     image.saveImage("brightness.png");
// }
int GetChoice()                                                                 //make a save image|| 
{                                                                               //                 ||
    int choice;                                                                 //                \  /
                                                                                //                 \/
    vector<string> choices = {"1- Load a new image", "2- GrayScale filter", "3- BlackAndWhite filter", "4- Flip image","5-" , "6- Crop image", "7- Invert image", "8- Rotate image", "9- Save image","10- Exit"};
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
    bool menuDisplayed = true;
    Image usedImage;
    string userInput;
    cout << "Input image" << endl;
    cin >> userInput;
    usedImage.loadNewImage(userInput);
    while (menuDisplayed)
    {
        switch (GetChoice())
        {
        case 1:
            cout<<"Input new image"<<endl;
            cin >> userInput;
            usedImage.loadNewImage(userInput);
            break;
        case 2:
            GrayScale(usedImage);
            break;
        case 3:
            BlackAndWhite(usedImage);
            break;
        case 4:
            int choice;
            cout<<"1- Flip image horizontally"<<"2- Flip image vertically"<<"3- cancel"<<endl;
            cin>> choice;
            switch(choice)
            {
                case 1:
                    FlipImageHorizontally(usedImage);
                    break;
                case 2:
                    FlipImageVertically(usedImage);
                    break;
                
            }
            break;
        case 5:
            
            break;
        case 6:
            int x, y, width, hight;
            cout << "Provide the starting point of x axis and y axis" << endl;
            cin >> x >> y;
            cout << "Provide the width and hight of the area you would like to crop"<<endl;
            cin >> width >> hight;
            CropImage(usedImage, x, y, width, hight);
            break;
        case 7:
            invert(usedImage);
            break;
        case 8:
            rotate(usedImage);
            break;
        case 9:
            string imageName;
            cout<<"Type the name that you would like to save the image with"<<endl;
            cin>>imageName;
            usedImage.saveImage(imageName);
            break;
        case 10:
            menuDisplayed = false;
            break;
        default:
            break;
        }
    }
    return 0;
}