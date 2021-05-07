
#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include <string>

class HealthBar {

public:
    int hp;
    int maxHp;
    NVGcontext* vg;
    bool isVisible;


    HealthBar(NVGcontext* vg);
    void setMaxHp(int newMax);
    void setHp(int hp);
    void initGivenPlayerType(PlayerType type);
    void decrementHp(int hp);
    void incrementHp(int hp);
    void draw(float x, float y, float w, float h);
    void setVisible(bool visible);


};