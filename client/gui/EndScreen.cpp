#include "EndScreen.h"

EndScreen::EndScreen(NVGcontext* vg) {
    this->vg = vg;
    this->isVisible = false;
    this->endTime = steady_clock::now();

    this->image = nvgCreateImage(vg, "shaders/fullscreen_elements/game_over.png", 0);
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
    auto duration = duration_cast<milliseconds>(steady_clock::now() - endTime);
    float alpha = std::fmin( ((float)duration.count()) / END_GAME_DELAY_INPUT, 1.0f);
    NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image, alpha);
    nvgBeginPath(vg);
    nvgRoundedRect(vg,  0, 0, windowWidth, windowHeight, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);

	nvgRestore(vg); 
}

void EndScreen::setVisible(bool visible) {
    isVisible = visible;
}

void EndScreen::setEndTime(steady_clock::time_point endTime) {
    this->endTime = endTime;
}