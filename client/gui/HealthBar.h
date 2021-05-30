#ifndef _GUI_HEALTHBAR_H_
#define _GUI_HEALTHBAR_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include <string>
#include <chrono>


// Flashing Damage Taken Constants for Graphics
#define HPBAR_DAMAGE_TAKEN_FLASHING_TIME_MS 1200
#define HPBAR_DAMAGE_TAKEN_FLASHING_INTERVAL_MS 200
#define HPBAR_SHADOW_HP_INTERVAL_MS 150

#define HEALTH_BAR_ALPHA 222.f


class HealthBar {

private:
    const NVGcolor shadowRedColor = nvgRGBA(235, 141, 141, HEALTH_BAR_ALPHA);
    const NVGcolor shadowWhiteColor = nvgRGBA(255, 255, 255, HEALTH_BAR_ALPHA);
    const NVGcolor redColor = nvgRGBA(211, 33, 33, HEALTH_BAR_ALPHA);
    const NVGcolor whiteColor = nvgRGBA(255, 255, 255, HEALTH_BAR_ALPHA);
    const NVGcolor armorColor = nvgRGBA(35, 192, 225, HEALTH_BAR_ALPHA);

public:
    int image,  imgWidth, imgHeight;
    int hp;
    int shadowHp;
    int maxHp;
    NVGcontext* vg;
    bool isVisible;
    NVGcolor barColor;
    NVGcolor shadowColor;
    std::chrono::steady_clock::time_point flashUntil;
    std::chrono::steady_clock::time_point lastFlash;
    std::chrono::steady_clock::time_point lastShadowChange;


    HealthBar(NVGcontext* vg);
    void setMaxHp(int newMax);
    void setHp(int hp);
    void initGivenPlayerType(PlayerType type);
    void decrementHp(int hp);
    void incrementHp(int hp);
    void draw(float x, float y, float w, float h);
    void setVisible(bool visible);

    // For taking damage
    void flashHealthBar();


};

#endif