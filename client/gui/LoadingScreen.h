#ifndef _GUI_LOADINGSCREEN_H_
#define _GUI_LOADINGSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>

#define NUM_IMAGES 2

class LoadingScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    int image [NUM_IMAGES];
    int imgWidth [NUM_IMAGES]; 
    int imgHeight [NUM_IMAGES];
    bool isLoaded;
    float loadingPercentage;
    int imgIndex;
    
    LoadingScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);

    void nextScreen();

};

#endif