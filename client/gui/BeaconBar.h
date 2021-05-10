
#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include <string>

class BeaconBar {

public:
    float captureAmount;
    NVGcontext* vg;
    bool isVisible;


    BeaconBar(NVGcontext* vg);
    void setAmount(float captureAmount);
    void draw(float x, float y, float w, float h);
    void setVisible(bool visible);
};