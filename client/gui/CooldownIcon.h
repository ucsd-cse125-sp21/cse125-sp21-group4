#ifndef _GUI_COOLDOWN_ICON_H_
#define _GUI_COOLDOWN_ICON_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include <string>
#include <chrono>

#define COOLDOWN_ICON_ALPHA 192.f


class CooldownIcon {

private:
    const NVGcolor shadowColor = nvgRGBA(110, 110, 110, COOLDOWN_ICON_ALPHA);
    const NVGcolor darkerShadowColor = nvgRGBA(41, 41, 41, COOLDOWN_ICON_ALPHA);
    const NVGcolor lightColor = nvgRGBA(200, 200, 200, 255);

public:
    NVGcontext* vg;
    bool isVisible;
    std::chrono::steady_clock::time_point lastActionTimePoint;
    std::chrono::steady_clock::time_point actionReadyTimePoint;
    PlayerType playerType;
    unsigned int cooldownInMilliseconds;

    int image, imgWidth, imgHeight;


    CooldownIcon(NVGcontext* vg, std::string actionName, PlayerType type, unsigned int cooldown_ms);
    void handleAction();
    void draw(float x, float y, float w, float h);
    void setVisible(bool visible);



};

#endif