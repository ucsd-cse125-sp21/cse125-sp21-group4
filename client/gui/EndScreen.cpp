#include "EndScreen.h"

EndScreen::EndScreen(NVGcontext* vg) {
    this->vg = vg;
    this->isVisible = false;

    this->image = nvgCreateImage(vg, "shaders/fullscreen_elements/game_over.png", NVG_IMAGE_REPEATY | NVG_IMAGE_REPEATX);
    nvgImageSize(vg, image, &imgWidth, &imgHeight);
}


void EndScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 

	// EndScreen Image
    // NVGpaint imgPaint =  nvgImagePattern(vg, (windowWidth - imgWidth) / 2, (windowHeight - imgHeight) / 2, imgWidth, imgHeight, 0.0f/180.0f*NVG_PI, this->image, 1.f);
    // nvgBeginPath(vg);
    // nvgRoundedRect(vg, (windowWidth - imgWidth) / 2, (windowHeight - imgHeight) / 2, imgWidth, imgWidth, 5);
    // nvgFillPaint(vg, imgPaint);
    // nvgFill(vg);
    NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, imgWidth, imgHeight, 0.0f/180.0f*NVG_PI, this->image, 1.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg,  0, 0, windowWidth, windowHeight, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);

	nvgRestore(vg); 
}

void EndScreen::setVisible(bool visible) {
    isVisible = visible;
}