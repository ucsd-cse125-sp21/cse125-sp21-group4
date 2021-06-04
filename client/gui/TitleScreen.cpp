#include "TitleScreen.h"

TitleScreen::TitleScreen(NVGcontext* vg) {
    this->vg = vg;
    this->isVisible = false;
    this->currentState = PRESS_ANY_KEY_SCREEN;

    this->image[0] = nvgCreateImage(vg, "shaders/fullscreen_elements/title/titlescreen.png", 0);
    nvgImageSize(vg, image[0], &imgWidth[0], &imgHeight[0]);

    this->image[1] = nvgCreateImage(vg, "shaders/fullscreen_elements/title/helpscreen.png", 0);
    nvgImageSize(vg, image[1], &imgWidth[1], &imgHeight[1]);

    this->image[2] = nvgCreateImage(vg, "shaders/fullscreen_elements/title/storyscreen.png", 0);
    nvgImageSize(vg, image[2], &imgWidth[2], &imgHeight[2]);

    this->image[3] = nvgCreateImage(vg, "shaders/fullscreen_elements/title/menu_option1.png", 0);
    nvgImageSize(vg, image[3], &imgWidth[3], &imgHeight[3]);

    this->image[4] = nvgCreateImage(vg, "shaders/fullscreen_elements/title/menu_option2.png", 0);
    nvgImageSize(vg, image[4], &imgWidth[4], &imgHeight[4]);

    this->image[5] = nvgCreateImage(vg, "shaders/fullscreen_elements/title/menu_option3.png", 0);
    nvgImageSize(vg, image[5], &imgWidth[5], &imgHeight[5]);

    this->image[6] = nvgCreateImage(vg, "shaders/fullscreen_elements/title/menu_option4.png", 0);
    nvgImageSize(vg, image[6], &imgWidth[6], &imgHeight[6]);
}


void TitleScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 

    NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image[currentState], 1.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg,  0, 0, windowWidth, windowHeight, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);
    

	nvgRestore(vg); 
}

void TitleScreen::setVisible(bool visible) {
    isVisible = visible;
}

void TitleScreen::nextOption() {
    if(currentState >= MENU_SCREEN_ENTER && currentState < MENU_SCREEN_STORY) {
        currentState++;
    } else if (currentState == MENU_SCREEN_STORY) {
        currentState = MENU_SCREEN_ENTER;
    }
}

void TitleScreen::prevOption() {
    if(currentState > MENU_SCREEN_ENTER && currentState <= MENU_SCREEN_STORY) {
        currentState--;
    } else if (currentState == MENU_SCREEN_ENTER) {
        currentState = MENU_SCREEN_STORY;
    }
}


void TitleScreen::returnToMenu() {
    currentState = MENU_SCREEN_ENTER;
}

void TitleScreen::handleKeyPress(int key) {

    if (currentState == PRESS_ANY_KEY_SCREEN) {
        currentState = MENU_SCREEN_ENTER;
        return;
    } 

    if(key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
        nextOption();
    } else if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
        prevOption();
    } else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE) {
        pressOption();
    } else if (key == GLFW_KEY_ESCAPE && (currentState == HOW_TO_PLAY_SCREEN  || currentState == STORY_SCREEN)) {
        returnToMenu();
    }
}

void TitleScreen::pressOption() {
    // does nothing, handled in TitleWindow
}