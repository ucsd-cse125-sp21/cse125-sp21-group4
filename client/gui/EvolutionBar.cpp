#include "EvolutionBar.h"

EvolutionBar::EvolutionBar(NVGcontext* vg) {
    this->vg = vg;
	this->evoLevel = 0.0f;
	this->isVisible = false;

    image = nvgCreateImage(vg, "shaders/hud_elements/evolution_bar.png", 0);
	// printf("image: %d\n", image);
	nvgImageSize(vg, image, &imgWidth, &imgHeight);
}

void EvolutionBar::setEvo(float evo) {
	this->evoLevel = evo;
}


void EvolutionBar::drawSingleBar(float x, float y, float w, float h) {

	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, w, h, 3);
	nvgFillColor(vg, purpleColor);
	nvgFill(vg);
}
void EvolutionBar::draw(float x, float y, float w, float h) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	float cornerRadius = 3.0f;
	nvgSave(vg);

	// Background image for the hp bar
    NVGpaint imgPaint =  nvgImagePattern(vg, x, y, w, h, 0.0f/180.0f*NVG_PI, image, EVO_BAR_ALPHA / 255.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w, h, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);


    // Draw the bar within the hud element
	const float maxBarHeight = h / 6.5;
	const float maxBarWidth = w / 6.8;
	const float widthBetweenSegments = w / 73;
	float barX = x + w / 6.3;
	float barY = y + h / 2.7;


	// segmented bar for monster evo
	if(evoLevel >= 0) {
		float barFillAmount = maxBarWidth * std::min(evoLevel, MONSTER_FIRST_STAGE_THRESHOLD);
		drawSingleBar(barX, barY, barFillAmount, maxBarHeight);
	}
	barX += maxBarWidth + widthBetweenSegments;
	
	if(evoLevel >= MONSTER_FIRST_STAGE_THRESHOLD) {
		float barFillAmount = maxBarWidth * (std::min(evoLevel, MONSTER_SECOND_STAGE_THRESHOLD) - MONSTER_FIRST_STAGE_THRESHOLD);
		drawSingleBar(barX, barY, barFillAmount, maxBarHeight);
	}
	barX += maxBarWidth + widthBetweenSegments;

	if(evoLevel >= MONSTER_SECOND_STAGE_THRESHOLD) {
		float barFillAmount = maxBarWidth * (std::min(evoLevel, MONSTER_THIRD_STAGE_THRESHOLD) - MONSTER_SECOND_STAGE_THRESHOLD);
		drawSingleBar(barX, barY, barFillAmount, maxBarHeight);
	}
	barX += maxBarWidth + widthBetweenSegments;
	
	if(evoLevel >= MONSTER_THIRD_STAGE_THRESHOLD) {
		float barFillAmount = maxBarWidth * (std::min(evoLevel, MONSTER_FOURTH_STAGE_THRESHOLD) - MONSTER_THIRD_STAGE_THRESHOLD);
		drawSingleBar(barX, barY, barFillAmount, maxBarHeight);
	}
	barX += maxBarWidth + widthBetweenSegments;

	
	if(evoLevel >= MONSTER_FOURTH_STAGE_THRESHOLD) {
		float barFillAmount = maxBarWidth * (std::min(evoLevel, MONSTER_FIFTH_STAGE_THRESHOLD) - MONSTER_FOURTH_STAGE_THRESHOLD);
		drawSingleBar(barX, barY, barFillAmount, maxBarHeight);
	}
	barX += maxBarWidth + widthBetweenSegments;
	nvgRestore(vg);
}

void EvolutionBar::setVisible(bool visible) {
    this->isVisible = visible;
}
