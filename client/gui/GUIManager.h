#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "HealthBar.h"
#include "../nanovg/nanovg.h"

class GUIManager    
{
public:
    int windowWidth;
    int windowHeight;
    int fbWidth;
    int fbHeight;
    float pixelRatio;
    NVGcontext* vg;

    // HUD Elements
    HealthBar* healthBar;
    


    GUIManager(int width, int height, int fbWidth, int fbHeight);
    void resizeWindow(int width, int height, int fbWidth, int fbHeight);
    void drawWindow(const char* title, float x, float y, float w, float h);
    void draw();
    void setHUDVisible(bool hudVisible);
};

#endif
