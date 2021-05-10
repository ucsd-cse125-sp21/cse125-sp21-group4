#ifndef _GUI_SELECTSCREEN_H_
#define _GUI_SELECTSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include "../ScreenElement.h"
#include <string>
#include <map>

class SelectScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    bool isMonster;
    PlayerType selected;
    std::map<PlayerType, bool> claimed;
    vector<ScreenElement*> selectScreenElements; 
    


    SelectScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void setMonster(bool);
    void handleRoleClaimed(PlayerType);
    void handleMouseSelect(float windowWidth, float windowHeight, int mouseX, int mouseY);
};

#endif