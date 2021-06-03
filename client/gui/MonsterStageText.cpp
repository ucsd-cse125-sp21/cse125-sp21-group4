#include "MonsterStageText.h"

MonsterStageText::MonsterStageText(NVGcontext* vg) {
    this->vg = vg;
    this->isVisible = false;
	currentStage = 0;


	for(int i = 0; i < NUM_MONSTER_STAGES; i++) {
		std::string stageFilePath = "shaders/hud_elements/stage" + std::to_string(i+1) +".png";
		this->image[i] = nvgCreateImage(vg, stageFilePath.c_str(), 0);
    	nvgImageSize(vg, image[i], &imgWidth[i], &imgHeight[i]);
	}
}

void MonsterStageText::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	float cornerRadius = 3.0f;
	nvgSave(vg);

	// Background image for the hp bar
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastDisplayed);
	if(currentStage >= 1 && duration.count() < MONSTER_NOTIFICATION_INTERVAL) {
		// NVGpaint imgPaint =  nvgImagePattern(vg, windowWidth / 4.2, -windowHeight / 19, windowWidth/2, windowHeight/2, 0.0f/180.0f*NVG_PI, this->image[currentStage - 1], 255.f);
		// nvgBeginPath(vg);
		// nvgRoundedRect(vg, windowWidth / 4.2,  -windowHeight / 19, windowWidth/2, windowHeight/2, 5);
		// nvgFillPaint(vg, imgPaint);
		// nvgFill(vg);
		NVGpaint imgPaint =  nvgImagePattern(vg, 0, windowHeight / 64, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image[currentStage - 1], 255.f);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 0,  windowHeight / 64, windowWidth, windowHeight, 5);
		nvgFillPaint(vg, imgPaint);
		nvgFill(vg);
	}
    

	nvgRestore(vg);
}

void MonsterStageText::setVisible(bool visible) {
    this->isVisible = visible;
}


bool MonsterStageText::flashMonsterEvolveEvent(int evolve) {
	if(evolve < 1 || evolve > 5) {
		return false;
	}
	currentStage = evolve;
	lastDisplayed = std::chrono::steady_clock::now();
	return true;
}