#include "EndScreen.h"

EndScreen::EndScreen(NVGcontext* vg) {
    this->vg = vg;
    this->isVisible = false;
    this->endTime = steady_clock::now();
    this->winner = -1;

    this->image[hunter_index] = nvgCreateImage(vg, "shaders/fullscreen_elements/win_hunters.png", 0);
    nvgImageSize(vg, image[hunter_index], &imgWidth[hunter_index], &imgHeight[hunter_index]);
    this->image[monster_index] = nvgCreateImage(vg, "shaders/fullscreen_elements/win_monsters.png", 0);
    nvgImageSize(vg, image[monster_index], &imgWidth[monster_index], &imgHeight[monster_index]);
}


void EndScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 

    // End Screen
    auto duration = duration_cast<milliseconds>(steady_clock::now() - endTime);
    float alpha = std::fmin( ((float)duration.count()) / END_GAME_DELAY_INPUT, 1.0f);

    // Hunter win condition
    if (winner == HUNTER_WINNER_COND) {
        NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image[hunter_index], alpha);
        nvgBeginPath(vg);
        nvgRoundedRect(vg,  0, 0, windowWidth, windowHeight, 5);
        nvgFillPaint(vg, imgPaint);
        nvgFill(vg);

    // Monster win condition
    } else if (winner == MONSTER_WINNER_COND) {
        NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image[monster_index], alpha);
        nvgBeginPath(vg);
        nvgRoundedRect(vg,  0, 0, windowWidth, windowHeight, 5);
        nvgFillPaint(vg, imgPaint);
        nvgFill(vg);
    }

	nvgRestore(vg); 
}

void EndScreen::setVisible(bool visible) {
    isVisible = visible;
}

void EndScreen::setEndTime(steady_clock::time_point endTime) {
    this->endTime = endTime;
}

void EndScreen::setWinner(int winner) {
    this->winner = winner;
}