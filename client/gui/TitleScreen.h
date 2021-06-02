#ifndef _GUI_TITLESCREEN_H_
#define _GUI_TITLESCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>

class TitleScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    int image, imgWidth, imgHeight;
    
    TitleScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);

};

#endif