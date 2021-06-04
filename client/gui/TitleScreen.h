#ifndef _GUI_TITLESCREEN_H_
#define _GUI_TITLESCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>

#define TITLE_NUM_IMAGES 7

const int PRESS_ANY_KEY_SCREEN = 0,
            HOW_TO_PLAY_SCREEN = 1,
            STORY_SCREEN = 2,
            MENU_SCREEN_ENTER = 3,
            MENU_SCREEN_EXIT = 4,
            MENU_SCREEN_HELP = 5,
            MENU_SCREEN_STORY = 6;
class TitleScreen {


public:
    NVGcontext* vg;
    bool isVisible;

    int image[TITLE_NUM_IMAGES], imgWidth[TITLE_NUM_IMAGES], imgHeight[TITLE_NUM_IMAGES];
    int currentState;
    
    TitleScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void nextOption();
    void prevOption();
    void pressOption();
    void returnToMenu();
    void handleKeyPress(int);

};

#endif