#ifndef _GUI_LOADINGSCREEN_H_
#define _GUI_LOADINGSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>

class LoadingScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    int image, imgWidth, imgHeight;
    bool isLoaded;
    float loadingPercentage;
    
    LoadingScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);

};

#endif