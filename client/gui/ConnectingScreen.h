#ifndef _GUI_TITLESCREEN_H_
#define _GUI_TITLESCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"

class ConnectingScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    
    ConnectingScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
};

#endif