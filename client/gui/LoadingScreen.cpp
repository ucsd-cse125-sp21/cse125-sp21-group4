#include "LoadingScreen.h"

LoadingScreen::LoadingScreen(NVGcontext* vg) {
    this->vg = vg;
    this->isVisible = false;
    this->isLoaded = false;
    // this->loadingPercentage = 0.f;

    this->image[0] = nvgCreateImage(vg, "shaders/fullscreen_elements/loadingscreen1.png", 0);
    nvgImageSize(vg, image[0], &imgWidth[0], &imgHeight[0]);

    this->image[1] = nvgCreateImage(vg, "shaders/fullscreen_elements/loadingscreen2.png", 0);
    nvgImageSize(vg, image[1], &imgWidth[1], &imgHeight[1]);

    this->imgIndex = 0;
}


void LoadingScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 


    NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image[imgIndex % NUM_LOADING_IMAGES], 1.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg,  0, 0, windowWidth, windowHeight, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);

	nvgRestore(vg); 
}

void LoadingScreen::setVisible(bool visible) {
    isVisible = visible;
}

void LoadingScreen::nextScreen() {
    imgIndex++;
    printf("Next loading screen [Index: %d mod %d]\n", imgIndex, NUM_LOADING_IMAGES);
}