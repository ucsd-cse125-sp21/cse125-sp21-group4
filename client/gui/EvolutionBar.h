#ifndef _GUI_EVOBAR_H_
#define _GUI_EVOBAR_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include <string>
#include <chrono>


#define EVO_BAR_ALPHA 222.f


class EvolutionBar {

private:
    const NVGcolor purpleColor = nvgRGBA(243, 30, 243, EVO_BAR_ALPHA);


    void drawSingleBar(float x, float y, float w, float h);
public:
    int image,  imgWidth, imgHeight;
    float evoLevel;
    NVGcontext* vg;
    bool isVisible;

    EvolutionBar(NVGcontext* vg);
    void setEvo(float evo);
    void draw(float x, float y, float w, float h);
    void setVisible(bool visible);


};

#endif