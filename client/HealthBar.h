
#include "../Core.h" // GL required for the NanoGUI.
#include "nanovg/nanovg.h"
#include <string>

class HealthBar {

protected:
    int hp;
    int maxHp;
    NVGcontext* vg;
    bool isVisible;
public:
    HealthBar(NVGcontext* vg);

    void setMaxHp(int newMax);
    void setHp(int hp);
    void draw(float x, float y, float w, float h);

    void setVisible(bool visible);


};