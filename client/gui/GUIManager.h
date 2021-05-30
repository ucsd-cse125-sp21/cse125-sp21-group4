#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "HealthBar.h"
#include "BeaconBar.h"
#include "EvolutionBar.h"
#include "MiniMap.h"
#include "SelectScreen.h"
#include "ConnectingScreen.h"
#include "EndScreen.h"
#include "SplashScreen.h"
#include "CooldownIcon.h"
#include "../nanovg/nanovg.h"

#define NUM_COOLDOWNS 2

enum CooldownID {
    NORMAL_ATTACK = 0,
    UNIQUE_ATTACK = 1,
};

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
    EvolutionBar* evoBar;
    MiniMap* miniMap;
    SelectScreen* selectScreen;
    ConnectingScreen* connectingScreen;
    EndScreen* endScreen;
    SplashScreen* splashScreen;
    CooldownIcon* cooldownIcons[NUM_COOLDOWNS]; // 2 because there's attack and unique attack.

    GUIManager(int width, int height, int fbWidth, int fbHeight);
    void resizeWindow(int width, int height, int fbWidth, int fbHeight);
    void drawWindow(const char* title, float x, float y, float w, float h);
    void drawCenterText(std::string, int, int);
    void draw();
    void setHUDVisible(bool visibility);
    void setSelectScreenVisible(bool visibility);
    void setConnectingScreenVisible(bool visibility);
    void setGameEndVisible(bool visibility);
    void setSplashScreenVisible(bool visibility);
    void setSplashLoaded(bool loaded);
    
    void initCooldownIcons(PlayerType type);
    void handleMouseSelect(int x, int y);
    void handleCooldownUpdate(CooldownID);
    void updateHUDPositions();

    void reset();
};

#endif
