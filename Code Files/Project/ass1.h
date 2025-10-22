#pragma once

#include "image_class.h"
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
void GrayScale(Image& image);
void BlackAndWhite(Image& image);
void invert(Image& image);
void rotate(Image& image, int deg);
void merge(Image& image, Image& mergeMe, int mergeChoice1, int mergeChoice2, int mergeChoice3, int mergeChoice4);
void FlipImageHorizontally(Image& image);
void FlipImageVertically(Image& image);
void CropImage(Image& image, int x, int y, int width, int height);
void AdjustBrightness(Image& image, float factor);
void blur(Image& image, int blurRadius);
void getRGB(unsigned char frameColor[3], int r, int g, int b);
void getColorchoice(unsigned char frameColor[3], int choiceColor, int r, int g, int b);
void frame(Image& image, int frameChoice, float thicknessPerct, int colorChoice, int r, int g, int b, int colorChoice2, int r2, int g2, int b2, int space, float thick);
void Resize(Image& image, int newWidth, int newHeight);
void merge(Image& image, Image& mergeMe, int mergeChoice1, int mergeChoice2);
void EdgeDetect(Image& image);
void RetroTV(Image& image);
void infrared(Image& image);
void skew(Image& image, float sliderDeg, int choice);
void AdjustWarmth(Image& image);
void OilPainting(Image& src);
void purple(Image& image);
void AdjustContrast(Image& image, float factor);
void pixelize(Image& image, int pixelSize);
void motionblur(Image& image, int direction, int blurRadius);
void Swirl(Image& image, float strength, bool goldenRatioChosen);
void Wavy(Image& image, int ampx, int  ampy, int frequency, bool vertical, bool horizontal, bool both);
