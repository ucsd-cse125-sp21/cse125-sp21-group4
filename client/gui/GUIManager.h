#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "HealthBar.h"
#include "BeaconBar.h"
#include "MiniMap.h"
#include "SelectScreen.h"
#include "ConnectingScreen.h"
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
    BeaconBar* beaconBar;
    MiniMap* miniMap;
    SelectScreen* selectScreen;
    ConnectingScreen* connectingScreen;

    GUIManager(int width, int height, int fbWidth, int fbHeight);
    void resizeWindow(int width, int height, int fbWidth, int fbHeight);
    void drawWindow(const char* title, float x, float y, float w, float h);
    void draw();
    void setHUDVisible(bool visibility);
    void setSelectScreenVisible(bool visibility);
    void setConnectingScreenVisible(bool visibility);
    void handleMouseSelect(int x, int y);
};

#endif
