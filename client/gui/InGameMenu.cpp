#include "InGameMenu.h"
#include "../Window.h"

InGameMenu::InGameMenu(NVGcontext* vg) {
    this->vg = vg;
    init();
}

void InGameMenu::init() {
	isVisible = false;
    isExiting = false;
}

void InGameMenu::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 
    


    // Background color
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 0, 0, windowWidth, windowHeight, 3);

    // if game already started, we want some transparency for the player so they can kinda see what's happening
    if(Window::gameStarted) {
        nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));

    // if not in game, take up the whole screen
    } else {
        nvgFillColor(vg, nvgRGBA(28, 30, 34, 255));
    }
    nvgFill(vg);
    nvgClosePath(vg);

    // instructions to enter or exit the game
    nvgFontSize(vg, windowHeight/32);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    if (isExiting) {
        
        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
        nvgText(vg, windowWidth/2, windowHeight/2, "Exiting game...", NULL);
    } else {

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
        nvgText(vg, windowWidth/2, windowHeight/2, "Press ESC to resume playing the game.", NULL);

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
        nvgText(vg, windowWidth/2, 3* windowHeight/4, "Press Enter to exit the game.", NULL);
    }

   

	nvgRestore(vg); 
}

void InGameMenu::setVisible(bool visible) {
    isVisible = visible;
}


void InGameMenu::handleKeyInput(int glfwKey, GLFWwindow * window) {

    // If input was a ESC.
    if(glfwKey == GLFW_KEY_ESCAPE) {
        isVisible = !isVisible;
        printf("%d", isVisible);
    }

    // If input was a ENTER.
    else if (glfwKey == GLFW_KEY_ENTER && isVisible)  {
        isExiting = true;
        Window::displayCallback(window);
        glfwSetWindowShouldClose(window, 1);
    }

}
