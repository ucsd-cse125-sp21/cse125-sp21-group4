#include "BeaconBar.h"

BeaconBar::BeaconBar(NVGcontext* vg) {
    this->vg = vg;
	isVisible = false;
	captureAmount = 0.f;
    
	image = nvgCreateImage(vg, "shaders/hud_elements/beacon_bar.png", 0);
	// printf("image: %d\n", image);
    nvgImageSize(vg, image, &imgWidth, &imgHeight);
}

void BeaconBar::setAmount(float captureAmount) {
    this->captureAmount = captureAmount;
}

void BeaconBar::draw(float x, float y, float w, float h) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	float cornerRadius = 3.0f;
	nvgSave(vg);


	// background color for teams
	const float maxCircleRadius = h / 1.4;
    nvgBeginPath(vg);
    nvgCircle(vg, x + w - w / 10.1, y + h / 2.5, maxCircleRadius);
    nvgFillColor(vg, monsterTeamColor);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgCircle(vg, x + w / 20,  y + h / 2.5, maxCircleRadius);
    nvgFillColor(vg, hunterTeamColor);
    nvgFill(vg);


	// Background image for the beacon bar
    NVGpaint imgPaint =  nvgImagePattern(vg, x, y, w, h, 0.0f/180.0f*NVG_PI, image, BEACON_BAR_ALPHA / 255.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w, h, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);



	// // Background color for the beacon
	// nvgBeginPath(vg);
	// nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	// nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
	// nvgFill(vg);

    // Fill beacon bar with specific color depending on the hunter or monster
    float beaconWidth;
	NVGcolor barColor;


	// Draw the bar within the hud element
	float barHeight = h / 8.5;
	float barWidth = w * 3.92 / 6;
	float barX = x + w / 7.2;
	float barY = y + h / 3.0;


	// purple bar for monster
	if (captureAmount < 0) { 
		beaconWidth = barWidth * captureAmount / MONSTER_BEACON_CAPTURE_THRESHOLD;
		barColor = monsterBeaconColor;

	// green bar for hunters
	} else { 
		beaconWidth = barWidth * captureAmount / HUNTER_BEACON_CAPTURE_THRESHOLD;
		barColor = hunterBeaconColor;
	}
	nvgBeginPath(vg);
	nvgRoundedRect(vg, barX, barY, beaconWidth, barHeight, cornerRadius);
	nvgFillColor(vg, barColor);
	nvgFill(vg);
    
	nvgRestore(vg);
}

void BeaconBar::setVisible(bool visible) {
    this->isVisible = visible;
}