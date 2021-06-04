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
	if(currentStage >= 1 ) {
		NVGpaint imgPaint =  nvgImagePattern(vg, 0, 0, windowWidth, windowHeight, 0.0f/180.0f*NVG_PI, this->image[currentStage - 1], 255.f);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 0,  0, windowWidth, windowHeight, 5);
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
	return true;
}