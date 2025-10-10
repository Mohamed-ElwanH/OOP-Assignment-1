flowchart TD

    A["main"] --> B["LoadCheck"]
    A --> C["SaveCheck"]
    A --> D["GetChoice"]
    A --> E["Filters"]
    A --> F["Frame Helpers"]
    A --> G["Utility / Input Flow"]

    %% Filters block
    E --> E1["GrayScale"]
    E --> E2["BlackAndWhite"]
    E --> E3["Invert"]
    E --> E4["Rotate"]
    E --> E5["FlipImageHorizontally"]
    E --> E6["FlipImageVertically"]
    E --> E7["CropImage"]
    E --> E8["Resize"]
    E --> E9["AdjustBrightness"]
    E --> E10["AdjustWarmth"]
    E --> E11["Blur"]
    E --> E12["EdgeDetect"]
    E --> E13["Merge"]
    E --> E14["RetroTV"]
    E --> E15["Infrared"]
    E --> E16["Skew"]
    E --> E17["OilPainting"]
    E --> E18["Purple"]
    E --> E19["Frame"]

    %% Frame helper functions
    F --> F1["getRGB"]
    F --> F2["getColorchoice"]

    %% Utility / flow handling
    G --> G1["menuDisplayed"]
    G --> G2["userInput"]
    G --> G3["imageName"]
    G --> G4["newWidth & newHeight"]
    G --> G5["user-driven filter selection"]
