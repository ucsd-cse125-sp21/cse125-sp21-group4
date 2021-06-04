#ifndef _GUI_ENDSCREEN_H_
#define _GUI_ENDSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>
#include <chrono>

// Delay end game button press
#define END_GAME_DELAY_INPUT 2000 // 2 seconds

using namespace std::chrono;

class EndScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    int image, imgWidth, imgHeight;
    steady_clock::time_point endTime;
    
    EndScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void setEndTime(steady_clock::time_point);

};

#endif