#ifndef _GUI_SELECTSCREEN_H_
#define _GUI_SELECTSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include "../ScreenElement.h"
#include <string>
#include <map>
#include <chrono>

class SelectScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    bool isMonster;
    bool hasClaimed;
    PlayerType selecting;
    std::map<PlayerType, bool> claimed;
    vector<ScreenElement*> selectScreenElements; 
    std::chrono::steady_clock::time_point timeSelectStarted;
    


    SelectScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void setMonster(bool);
    void handleRoleClaimed(PlayerType);
    void handleSelecting(PlayerType roleSelected);
    void startTimer();
};

#endif