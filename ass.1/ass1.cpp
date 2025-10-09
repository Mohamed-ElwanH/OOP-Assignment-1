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
    void LoadCheck(string userInput, Image& usedImage)
    {
        try
    {
            usedImage.loadNewImage(userInput);
            cout << "Image loaded successfully" << endl;
            return;
        }
        catch(

        const invalid_argument& error)
    {
            cout << error.what() << "\nTry again\n"
             << endl;
            cin >> userInput;
            LoadCheck(userInput, usedImage);
        }
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
    void rotate(Image& image) // Rotates the image by 90, 180 or 270 degrees depending on the user's choice
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
    void FlipImageHorizontally(Image& image) // Flips the image horizontally
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
    void FlipImageVertically(Image& image) // Flips the image vertically
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
    void CropImage(Image& image, int x, int y, int width, int height) // Crops the image based on the user's input
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
    void AdjustBrightness(Image& image, float factor) //increases or decreases the brightness based on the user's input (1.5 for 50% increase and 0.5 for 50% decrease)
    {
        
        for (int i = 0; i < image.width; i++)
        {
            for (int j = 0; j < image.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    int value = static_cast <
                    int > (image(i, j, k) * factor);
               
                    image(i, j, k) = min(255, max(0, value));
                }
            }
        }
    }
    void merge(Image& image) // Merges two images by averaging their pixel values
    {
        Image mergeMe;
        string mergename;
        cout << "Input the name of the image you want to merge" << endl;
        cin >> mergename;
        LoadCheck(mergename, mergeMe);

        // Ensure both images have the same dimensions
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
        image = mergedImage;
    }
    void EdgeDetect(Image& image)
    {
        Image copy(image.width, image.height);
        copy = image;
        GrayScale(image);
       int Gx[3][3] = { 
                {-1, 0, 1},
                {-2, 0, 2},
                {- 1, 0, 1}
        };
        int Gy[3][3] = { 
                {1, 2, 1},
                {0, 0, 0},
                {-1, -2, -1}
        };
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
        //int threshold;
        //cin >> threshold;
         
        //            if (i == 0 || j == 0 || i == image.width - 1 || j == image.height - 1)
        //            {
        //                continue; // Skip border pixels
        //            }
        //            if (abs(image(i, j, k) - image(i + 1, j, k)) > threshold || abs(image(i, j, k) - image(i, j + 1, k)) > threshold)
        //            {
        //                image(i, j, k) = 0;
                        
        //            }
        //            else
        //            {
        //                image(i, j, k) = 255;
        //            }
        //        }
        //    }
        //}
    }
    void blur(Image& image)
    {
        Image blurred

        (image.width, image.height);
        int percentage, avgBlur;
        cout << "blur percentage \n";
        cin >> percentage;
        //avgBlur = (percentage * min(image.width, image.height)) / 100; //translates percentage of blur to average pixels 
        avgBlur = 1;
     
        for (int i = 0; i < image.width; i++)
        {
            for (int j = 0; j < image.height; j++)
            {
                int cntr = 0;
                int rgb[3] = { 0, 0, 0 }; //store sum of rgb separately
                for (int m = -avgBlur; m <= avgBlur; m++)
                { //box blur method
                    for (int n = -avgBlur; n <= avgBlur; n++)
                    {
                        
                        int ni = i + m; //horizontal neighbors 
                        int nj = j + n; //vertical neighbors

                        if (ni >= 0 && nj >= 0 && nj < image.height && ni < image.width)
                        { //edge check
                            for (int k = 0; k < 3; k++)
                            {
                                rgb[k] += image(ni, nj, k);
                            }
                            cntr++; //counts valid pixels
                        }
                     
                     
            


                    }
               
                
                }
                for (int k = 0; k < 3; k++)
                {
                    blurred(i, j, k) = round(rgb[k] / cntr);
                }
                
            }
              
 
        }
        image = blurred;
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
    void skew(Image& image) // Skews the image to the right
    {
        
        
        double skewdeg;
        cout << "Enter skew degree (in degrees): ";
        cin >> skewdeg;
        int newI = 0;
        skewdeg = skewdeg * (3.14159265358979323846 / 180.0); // Convert degrees to radians
        
        int newWidth = image.width + abs(image.height * tan(skewdeg));
        Image skewedImage(newWidth, image.height);
        //int y = abs(image.height * tan(skewdeg));
        //cout << "increasing by " "" << y << endl;
        for (int i = 0 ; i < image.width ; i++)
        {
            for (int j = 0; j < image.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    //int extra = j * tan(skewdeg);
                    
                    
                    newI = (i + (abs(tan(skewdeg)) * (image.height - 1 - j)));
                    //if (newI < 0)
                    //{
                        
                    //    newI = -1 * newI;
                    //}
                    //if (i == 0 || j == 0 || i == image.width - 1 || j == image.height - 1)
                    //{
                    //    continue; // Skip border pixels
                    //}
              
                    
                    skewedImage(newI, j, k) = image(i, j, k);
                }
                
            }
        }
        image = skewedImage;
    }
   
    int GetChoice() // Displays the menu and gets the user's choice
    {
        int choice;
        vector<string> choices =
        {
            "1- Load a new image",
            "2- GrayScale filter",
            "3- BlackAndWhite filter",
            "4- Flip image",
            "5- Brightness Adjustment",
            "6- Crop image",
            "7- Invert image",
            "8- Rotate image",
            "9- Edge detection",
            "10- Merge",
            "11- Save image",
            "12- Exit",
            "13- Blur Image",
            "14- Infrared filter"
        };
        cout << "Choose" << endl;
        for (int i = 0; i < choices.size(); i++)
        {
            cout << choices[i] << endl;
        }
        cin >> choice;
        return choice;
    }
    
    void SaveCheck(Image usedImage)
    {
        try
    {
            string imageName;
            cin >> imageName;
            usedImage.saveImage(imageName);
            cout << "Saved Successfully" << endl;
            return;
        }
        catch(

        const invalid_argument& x)
    {

            SaveCheck(usedImage);
        }
    }
    int main() //main program
    {
        string merged;
        bool menuDisplayed = true;
        Image usedImage;
        string userInput;
        cout << "Input image" << endl;
        cin >> userInput;
        LoadCheck(userInput, usedImage);
        
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
                    EdgeDetect(usedImage);
                    break;
                case 10:
                    merge(usedImage);
                    break;
                case 11:
                    cout << "Type the name that you would like to save the image with" << endl;
                    SaveCheck(usedImage);
                    break;
                case 12:
                    menuDisplayed = false;
                    break;
                case 13:
                    blur(usedImage);
                        
                        // Optional: Check if usedImage.imageData is valid
                    if (usedImage.imageData == nullptr)
                    {
                        cout << "Error: Image data is empty after blur!" << endl;
                    }
                    cout << usedImage.width << " " << usedImage.height << endl;
                    
                    break;
                case 14:
                    infrared(usedImage);
                    break;
                case 15:
                    skew(usedImage);
                    break;
                default:
                    break;
            }
        }
        return 0;
    }
    