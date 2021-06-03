#ifndef _GUI_CONNECTSCREEN_H_
#define _GUI_CONNECTSCREEN_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include <GLFW/glfw3.h>
#include <string>

#define NUM_CONNECTING_IMAGES 2


class ConnectingScreen {

public:
    NVGcontext* vg;
    bool isVisible;
    bool hasConnectedToServer; // tells if they've established connection
    bool connecting; // tells if the screen is trying to connect.
    std::string inputtedText;

    
    int image [NUM_CONNECTING_IMAGES];
    int imgWidth [NUM_CONNECTING_IMAGES]; 
    int imgHeight [NUM_CONNECTING_IMAGES];
    
    ConnectingScreen(NVGcontext* vg);
    void draw(float windowWidth, float windowHeight);
    void setVisible(bool visible);
    void handleKeyInput(int glfwKey, GLFWwindow * window);

    void init();
};

#endif