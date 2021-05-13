#include "ConnectingScreen.h"
#include "../Window.h"

ConnectingScreen::ConnectingScreen(NVGcontext* vg) {
    this->vg = vg;
	isVisible = false;
    hasConnectedToServer = false;
    connecting = false;
    inputtedText = std::string();
}


void ConnectingScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 
    

    // Display waiting for other players to join:
    if (hasConnectedToServer) {

        // Background color
        nvgBeginPath(vg);
        nvgRoundedRect(vg, 0, 0, windowWidth, windowHeight, 3);
        nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
        nvgFill(vg);
        nvgClosePath(vg);

        // awaiting for other players text
        nvgFontSize(vg, windowHeight/32);
        nvgFontFace(vg, "sans-bold");
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
        nvgText(vg, windowWidth/2, windowHeight/2, "Waiting for other players to join...", NULL);

    // Otherwise tell them to input a server IP address
    } else {

        // Background color
        nvgBeginPath(vg);
        nvgRoundedRect(vg, 0, 0, windowWidth, windowHeight, 3);
        nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
        nvgFill(vg);
        nvgClosePath(vg);

        // Background color for string
        nvgBeginPath(vg);
        nvgRoundedRect(vg, windowWidth / 2 - 300, windowHeight / 2 - 50, 600, 100, 3);
        nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
        nvgFill(vg);
        nvgClosePath(vg);

        // Instructions to enter the server IP
        nvgFontSize(vg, windowHeight/32);
        nvgFontFace(vg, "sans-bold");
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
        nvgText(vg, windowWidth/2, windowHeight/4, "Please enter the Server's IPv4 address (e.g. 127.0.0.1). Press Enter to connect.", NULL);

        
        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
        nvgText(vg, windowWidth/2, windowHeight/2, inputtedText.c_str(), NULL);

        // If currently connecting, print awaiting server response.
        if(connecting) {
            std::string establishingConnection = "Attempting to establish connection to: " + inputtedText + " ... Please wait.";
            nvgFontBlur(vg, 0);
            nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
            nvgText(vg, windowWidth/2, 7 * windowHeight/8, establishingConnection.c_str(), NULL);

        }


    }

	nvgRestore(vg); 
}

void ConnectingScreen::setVisible(bool visible) {
    isVisible = visible;
}

void ConnectingScreen::handleKeyInput(int glfwKey, GLFWwindow * window) {

    // If input was a number.
    if(glfwKey >= GLFW_KEY_0 && glfwKey <= GLFW_KEY_9) {
        inputtedText += std::to_string(glfwKey - GLFW_KEY_0);
    }

    // If input was a period.
    else if (glfwKey == GLFW_KEY_PERIOD)  {
        inputtedText += '.';
    }
    
    // If input was an enter key 
    else if (glfwKey == GLFW_KEY_ENTER) {
        if(hasConnectedToServer) {
            return;
        }

        // Call displayCallback just to print out the "Establishing connection message".
        connecting = true;
        Window::displayCallback(window);

        // Display connecting...
        if(!hasConnectedToServer) {
            bool establishedConnection =  Window::connectCommClient(inputtedText);
            if(establishedConnection) {
                hasConnectedToServer = true;
            } 
            connecting = false;
        }
    }

    // If input was a backspace
    else if (glfwKey == GLFW_KEY_BACKSPACE) {
        if(!inputtedText.empty()) {
            inputtedText.pop_back();
        }
    }

}