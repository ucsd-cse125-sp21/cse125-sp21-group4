#ifndef _GUI_TITLESCREEN_H_
#define _GUI_TITLESCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>
#include <string>

class ConnectingScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    bool hasConnectedToServer; // tells if they've established connection
    bool connecting; // tells if the screen is trying to connect.
    std::string inputtedText;
    
    ConnectingScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void handleKeyInput(int glfwKey, GLFWwindow * window);
};

#endif