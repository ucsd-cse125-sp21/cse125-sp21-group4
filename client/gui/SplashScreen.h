#ifndef _GUI_SPLASHSCREEN_H_
#define _GUI_SPLASHSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>

class SplashScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    int image, imgWidth, imgHeight;
    bool isLoaded;
    float loadingPercentage;
    
    SplashScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void setSplashLoaded(bool isLoaded);

};

#endif