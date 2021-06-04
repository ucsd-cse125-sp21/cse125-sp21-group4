#ifndef _GUI_ENDSCREEN_H_
#define _GUI_ENDSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>
#include <chrono>

// Delay end game button press
#define END_GAME_DELAY_INPUT 2000 // 2 seconds
#define NUM_END_SCREENS 2


#define HUNTER_WINNER_COND 0
#define MONSTER_WINNER_COND 1
#define NO_WINNER -1

using namespace std::chrono;

class EndScreen {

public:
    const int hunter_index = 0;
    const int monster_index = 1;

    NVGcontext* vg;
    bool isVisible;
    int image[NUM_END_SCREENS], imgWidth[NUM_END_SCREENS], imgHeight[NUM_END_SCREENS];
    steady_clock::time_point endTime;
    int winner;
    
    EndScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void setEndTime(steady_clock::time_point);
    void setWinner(int);

};

#endif