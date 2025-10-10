mermaid
flowchart TD

    A[main()] --> B[LoadCheck(string, Image&)]
    A --> C[SaveCheck(Image, string)]
    A --> D[GetChoice()]
    A --> E[Filters]
    A --> F[Frame Helpers]
    A --> G[Utility / Input Flow]

    %% Filters block
    E --> E1[GrayScale(Image&)]
    E --> E2[BlackAndWhite(Image&)]
    E --> E3[invert(Image&)]
    E --> E4[rotate(Image&)]
    E --> E5[FlipImageHorizontally(Image&)]
    E --> E6[FlipImageVertically(Image&)]
    E --> E7[CropImage(Image&, x, y, w, h)]
    E --> E8[Resize(Image&, newWidth, newHeight)]
    E --> E9[AdjustBrightness(Image&, factor)]
    E --> E10[AdjustWarmth(Image&)]
    E --> E11[blur(Image&)]
    E --> E12[EdgeDetect(Image&)]
    E --> E13[merge(Image&)]
    E --> E14[RetroTV(Image&)]
    E --> E15[infrared(Image&)]
    E --> E16[skew(Image&)]
    E --> E17[OilPainting(Image&)]
    E --> E18[purple(Image&)]
    E --> E19[frame(Image&)]

    %% Frame helper functions
    F --> F1[getRGB(unsigned char[3])]
    F --> F2[getColorchoice(unsigned char[3])]

    %% Utility / flow handling
    G --> G1[menuDisplayed (bool)]
    G --> G2[userInput (string)]
    G --> G3[imageName (string)]
    G --> G4[newWidth, newHeight (int)]
    G --> G5[user-driven filter selection (switch-case 1–21)]
