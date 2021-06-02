#ifndef _GUI_INGAMESCREEN_H_
#define _GUI_INGAMESCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>
#include <string>
#include <thread>

class InGameMenu {

public:
    NVGcontext* vg;
    bool isVisible;
    bool isExiting;
    
    InGameMenu(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void handleKeyInput(int glfwKey, GLFWwindow * window);

    void init();
};

#endif