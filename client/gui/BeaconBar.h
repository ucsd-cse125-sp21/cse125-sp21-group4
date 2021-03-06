#ifndef _GUI_BEACONBAR_H_
#define _GUI_BEACONBAR_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include <string>

#define BEACON_BAR_ALPHA 222.f

class BeaconBar {

private:
    const NVGcolor monsterBeaconColor = nvgRGBA(75,41,65, BEACON_BAR_ALPHA);
    const NVGcolor hunterBeaconColor = nvgRGBA(55, 70, 38, BEACON_BAR_ALPHA);
    const NVGcolor monsterTeamColor = nvgRGBA(75,41,65, 255 - BEACON_BAR_ALPHA);
    const NVGcolor hunterTeamColor = nvgRGBA(55, 70, 38, 255 - BEACON_BAR_ALPHA);

public:
    int image,  imgWidth, imgHeight;
    float captureAmount;
    NVGcontext* vg;
    bool isVisible;


    BeaconBar(NVGcontext* vg);
    void setAmount(float captureAmount);
    void draw(float x, float y, float w, float h);
    void setVisible(bool visible);
};

#endif