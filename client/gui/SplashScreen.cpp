#include "SplashScreen.h"

SplashScreen::SplashScreen(NVGcontext* vg) {
    this->vg = vg;
    this->isVisible = false;
    this->isLoaded = false;
    // this->loadingPercentage = 0.f;

    this->image = nvgCreateImage(vg, "shaders/fullscreen_elements/splashscreen.png", 0);
    nvgImageSize(vg, image, &imgWidth, &imgHeight);
}


void SplashScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 

    // Background color for the text
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 0, 0, windowWidth, windowHeight, 3);
    nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
    nvgFill(vg);
    nvgClosePath(vg);

    NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image, 1.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg,  0, 0, windowWidth, windowHeight, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);

    if(!isLoaded) {
        // Instructions for player to wait
        nvgFontSize(vg, windowHeight/16);
        nvgFontFace(vg, "sans-bold");
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 192));
        nvgText(vg, windowWidth/2, windowHeight * 2 / 3, "Loading..." , NULL);

        // Draw the loading bar in the splash screen
        // float barWidth = loadingPercentage * windowWidth / 2;
        // nvgBeginPath(vg);
        // nvgRoundedRect(vg, windowWidth/4, windowHeight * 3 / 4, barWidth, windowHeight / 10, cornerRadius);
        // nvgFillColor(vg, nvgRGBA(23, 145, 23, 192));
        // nvgFill(vg);
    }
    

	nvgRestore(vg); 
}

void SplashScreen::setVisible(bool visible) {
    isVisible = visible;
}

void SplashScreen::setSplashLoaded(bool isLoaded) {
    this->isLoaded = isLoaded;
}
// void SplashScreen::setPercent(float percent) {
//     this->loadPercentage = percent;
// }