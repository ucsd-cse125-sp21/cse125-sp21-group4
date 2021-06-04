#ifndef _GUI_MOSNTERSTAGETEXT_H_
#define _GUI_MOSNTERSTAGETEXT_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include <string>
#include <chrono>

#define MONSTER_NOTIFICATION_INTERVAL 30000 // 15000 ms = 15s

class MonsterStageText {

public:
    int image [NUM_MONSTER_STAGES];
    int imgWidth [NUM_MONSTER_STAGES]; 
    int imgHeight [NUM_MONSTER_STAGES];

    NVGcontext* vg;
    bool isVisible;
    std::chrono::steady_clock::time_point lastDisplayed;
    int currentStage;


    MonsterStageText(NVGcontext* vg);
    void draw(float w, float h);
    void setVisible(bool visible);

    // For evolve events
    bool flashMonsterEvolveEvent(int evolve);


};

#endif