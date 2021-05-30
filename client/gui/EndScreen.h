#ifndef _GUI_ENDSCREEN_H_
#define _GUI_ENDSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>

class EndScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    int image, imgWidth, imgHeight;
    
    EndScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);

};

#endif