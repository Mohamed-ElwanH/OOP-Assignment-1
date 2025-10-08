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
using namespace std;
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
                int value = static_cast<int>(image(i, j, k) * factor);

                image(i, j, k) = std::min(255, std::max(0, value));
            }
        }
    }
}



void blur(Image &image ){
    Image blurredh (image.width,image.height) ,blurredv (image.width,image.height);
    int blurRadius;
    cout<<"Enter blur radius 0-100 \n";
    cin>>blurRadius;
    
    //horizontal run
     for( int j = 0; j< image.height; j++ ){
       
        for(int k = 0; k < 3; k++ ){ //loops over one color in all rows
             int sum =0 ,cntr=0; //sum of the pixels in the window and counter for valid pixels
             for(int i =0 ; i < image.width && i < blurRadius ; i++ ){ //fills the first window
                 sum += image(i,j,k);
                 cntr++;
             }
             for(int i =0 ; i < image.width ; i++ ){ 
                 int avg= round(sum/cntr); //assigns the average to the blurred image
                 if( avg > 255 ) avg = 255;
                 if( avg < 0 ) avg = 0;
                  blurredh(i,j,k) = avg;
                 
                 if( i - blurRadius >= 0 ) {sum -= image(i- blurRadius,j ,k); cntr--;} //removes the old pixel
                 if( i + blurRadius + 1 < image.width ) {sum += image(i+ blurRadius + 1,j ,k); cntr++;}; //adds the new pixel
             }  
             
            

        }
    }
    //vertical run

    for( int i = 0; i < image.width; i++ ){
       
        for(int k = 0; k < 3; k++ ){ //loops over one color in all columns
             int sum = 0  , cntr=0;
             for(int j =0 ; j < image.height && j < blurRadius ; j++ ){ //fills the first window
                 sum += blurredh(i,j,k);
                 cntr++;
             }
             for(int j =0 ; j < image.height ; j++ ){ 
                int avg= round(sum/cntr); 
                if( avg > 255 ) avg = 255;
                 if( avg < 0 ) avg = 0;
                 blurredv(i,j,k) =avg;
                 if( j - blurRadius >= 0 ) {sum -= blurredh(i,j - blurRadius,k); cntr--;} //removes the old pixel
                 if( j + blurRadius + 1 < image.height ) {sum += blurredh(i,j + blurRadius + 1,k); cntr++;}//adds the new pixel
             }  
             
            

        }
    }
    
    image=blurredv;
    
    
} 

//scrapped code for box blur

// void blur(Image &image) {
//     Image blurredh(image.width, image.height), blurredv(image.width, image.height);
//     int blurRadius;
//     cout << "Enter blur radius: ";
//     cin >> blurRadius;

//     int diameter = 2 * blurRadius + 1;

//     // --- Horizontal pass ---
//     for (int y = 0; y < image.height; y++) {
//         for (int c = 0; c < 3; c++) { // R, G, B
//             int sum = 0;

//             // initialize first window
//             for (int x = 0; x <= blurRadius && x < image.width; x++)
//                 sum += image(x, y, c);

//             for (int x = 0; x < image.width; x++) {
//                 blurredh(x, y, c) = sum / (2 * blurRadius + 1);

//                 // slide window
//                 if (x - blurRadius >= 0)
//                     sum -= image(x - blurRadius, y, c);
//                 if (x + blurRadius + 1 < image.width)
//                     sum += image(x + blurRadius + 1, y, c);
//             }
//         }
//     }

//     // --- Vertical pass ---
//     for (int x = 0; x < image.width; x++) {
//         for (int c = 0; c < 3; c++) { // R, G, B
//             int sum = 0;

//             // initialize first window
//             for (int y = 0; y <= blurRadius && y < image.height; y++)
//                 sum += blurredh(x, y, c);

//             for (int y = 0; y < image.height; y++) {
//                 blurredv(x, y, c) = sum / (2 * blurRadius + 1);

//                 // slide window
//                 if (y - blurRadius >= 0)
//                     sum -= blurredh(x, y - blurRadius, c);
//                 if (y + blurRadius + 1 < image.height)
//                     sum += blurredh(x, y + blurRadius + 1, c);
//             }
//         }
//     }

//     image = blurredv;
// }
    //avgBlur = (percentage * min(image.width,image.height))/ 100; //translates percentage of blur to average pixels 
    //avgBlur=5;
    
    
    //  for (int i = 0; i < image.width; i++)
    // {
    //     for (int j = 0; j < image.height; j++)
    //     {
    //         int cntr=0 ,avgr=0,avgb=0,avgg=0;
    //         int rgb[3] ={0,0,0}; //store sum of rgb separately
    //         for(int m = -avgBlur;m <= avgBlur; m++ ){ //box blur method
    //             for(int n = -avgBlur;n <= avgBlur; n++ ){

    //                 int ni = i + m; //horizontal neighbors 
    //                 int nj = j + n; //vertical neighbors

    //                 if ( ni>=0 && nj>=0 && nj<image.height && ni<image.width ){ //edge check
    //                      for (int k = 0; k < 3; k++)
    //                      {
    //                         rgb[k] += image(ni, nj, k);
                             
    //                      }
    //                      cntr++; //counts valid pixels
    //                 }
                     
                     
            


    //             }
               
                
    //         }
    //         for( int k = 0; k < 3; k++){    
    //             blurred(i,j,k) = round(rgb[k]/cntr);
    //         }
                
    //     }
              
 
    // }
    // image=blurred;

    


    

void getFramecolor(unsigned char frameColor[3]){
    cout<<"enter rgb values from 0-255 \n";

    for(int i = 0; i < 3; i++){
        int inpt;
        cin>>inpt;
        if(inpt < 0 || inpt > 255) {
            cout << "Invalid input, try again: ";
            i--; // Retry this input
            continue;
        }
        frameColor[i] = static_cast<unsigned char>(inpt);
    }
}
void frame(Image &image){
    int choice;
    cout<<"1-border \n2-simple lines frame \n";
    cin>>choice;
    
    if(choice==1){
        float percentage;
        cout<<"enter the thickness of the border 1-40 \n";
        cin>>percentage;
        percentage /= 100;

        int border = percentage * min(image.height , image.width);
    
        int frameWidth = image.width + 2*border;
        int frameHeight = image.height + 2*border;
        Image frame (frameWidth,frameHeight) ;
    
        unsigned char frameColor[3] ={0,0,0}; //rgb value control
        
        getFramecolor(frameColor);
        for (int i = 0; i < frame.width; i++) //fill the border with the chosen color
        {
            for (int j = 0; j < frame.height; j++)
            {
                for (int k = 0; k < 3; k++){
                    frame(i,j,k) = frameColor[k];

                }
            }
        } 

        int lineThickness = border/2;

        

        for (int i = 0; i < image.width; i++) //place the image in the middle of the frame
        {
            for (int j = 0; j < image.height; j++)
            {
                for(int k = 0; k < 3 ;k++){
                    
                    frame(i+border, j + border ,k) = image(i,j,k);
                                                                                        
                }
                
            }
        }
        
        

        
        image=frame;


            
        }
    
    else if (choice==2){
        unsigned char frameColor[3] ={0,0,0}; //rgb value control
        int offset = 0.05 * min(image.height , image.width);
        int lineThickness = offset/3;
        int gap = lineThickness/2; //gap between lines
        
        getFramecolor(frameColor);
        

         for (int i = 0; i < image.width; i++) //place the image in the middle of the frame
        {
            for (int j = 0; j < image.height; j++)
            {
                
                    bool isFramePixel = false;
                    

                    if (
                        // Left vertical lines
                        (i > offset && i < offset + lineThickness) ||
                        (i > offset + lineThickness + gap && i < offset + gap + 2*lineThickness) ||
                        // Right vertical lines
                        (i > image.width - offset - lineThickness && i < image.width - offset) ||
                        (i > image.width - offset - 2*lineThickness - gap && i < image.width - offset - lineThickness - gap) ||
                        // Top horizontal lines
                        (j > offset && j < offset + lineThickness) ||
                        (j > offset + lineThickness + gap && j < offset + gap + 2*lineThickness) ||
                        // Bottom horizontal lines
                        (j > image.height - offset - lineThickness && j < image.height - offset) ||
                        (j > image.height - offset - 2*lineThickness - gap && j < image.height - offset - lineThickness - gap)
                        ) {
                            isFramePixel = true;
                                }
                    if (isFramePixel){
                        for(int k = 0; k < 3 ;k++){
                            image(i,j,k) = frameColor[k];
                        }

                    }




                    
                    
                                                                                        
                }
                
            }
        


    }
} 

void RetroTV(Image &image) {

     for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {

                if(j%3==0) image(i, j, k) *= 0.5;
                else {int value = static_cast<int>(image(i, j, k) * 1.5);

                image(i, j, k) = std::min(255, std::max(0, value));}
                
            }
        }
    }
    
}

int GetChoice() // Displays the menu and gets the user's choice
{
    int choice;
    vector<string> choices = {"1- Load a new image", "2- GrayScale filter", "3- BlackAndWhite filter", "4- Flip image", "5- Brightness Adjustment", "6- Crop image", "7- Invert image", "8- Rotate image", "9- Save image", "10- Exit","11-blur","12-frame","13-Retro TV effect"};
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
    bool menuDisplayed = true;
    bool loadImage = false;
    Image usedImage;
    string userInput;
    cout << "Input image" << endl;
    cin >> userInput;
    while (!loadImage) // Loops until the image entered is valid
    {
        try
        {
            usedImage.loadNewImage(userInput);
            loadImage = true;
            cout << "Image loaded successfully" << endl;
            break;
        }
        catch (

            const exception &error)
        {
            cout << error.what() << "\nTry again\n"
                 << endl;
            cin >> userInput;
        }
    }
    loadImage = false;
    string imageName;
    bool wrongName = false;

    while (menuDisplayed) // Menu loop until user chooses to exit
    {
        switch (GetChoice())
        {
        case 1: // Load new image
            cout << "Input image" << endl;
            cin >> userInput;
            while (!loadImage)
            {
                try
                {
                    usedImage.loadNewImage(userInput);
                    loadImage = true;
                    cout << "Image loaded successfully" << endl;
                }
                catch (

                    const exception &error)
                {
                    cout << error.what() << "\nTry again\n"
                         << endl;
                    cin >> userInput;
                    loadImage = false;
                }
            }
            loadImage = false;
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
            wrongName = false;
            cout << "Type the name that you would like to save the image with" << endl;
            while (!wrongName)
            {
                wrongName = true;
                cin >> imageName;
                try
                {
                    usedImage.saveImage(imageName);
                    cout << "Saved Successfully" << endl;
                }
                catch (

                    const std::exception &x)
                {
                    wrongName = false;
                }
            }
            wrongName = true;
            break;
        case 10:
            menuDisplayed = false;
            break;
        case 11:
            blur(usedImage);
            break;
        case 12:
            frame(usedImage);
            break;
        case 13:
            RetroTV(usedImage);
            break;
        default:
            break;
        }
    }
    return 0;
}