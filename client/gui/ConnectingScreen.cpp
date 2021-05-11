#include "ConnectingScreen.h"

ConnectingScreen::ConnectingScreen(NVGcontext* vg) {
    this->vg = vg;
	isVisible = false;
}


void ConnectingScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 
    
    // Display waiting for other players to join:
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

	nvgRestore(vg); 
}

void ConnectingScreen::setVisible(bool visible) {
    isVisible = visible;
}