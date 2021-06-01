#include "LoadingScreen.h"

LoadingScreen::LoadingScreen(NVGcontext* vg) {
    this->vg = vg;
    this->isVisible = false;
    this->isLoaded = false;
    // this->loadingPercentage = 0.f;

    this->image = nvgCreateImage(vg, "shaders/fullscreen_elements/loadingscreen.png", 0);
    nvgImageSize(vg, image, &imgWidth, &imgHeight);
}


void LoadingScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 


    NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image, 1.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg,  0, 0, windowWidth, windowHeight, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);

	nvgRestore(vg); 
}

void LoadingScreen::setVisible(bool visible) {
    isVisible = visible;
}