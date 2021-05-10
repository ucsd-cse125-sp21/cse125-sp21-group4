#include "BeaconBar.h"

BeaconBar::BeaconBar(NVGcontext* vg) {
    this->vg = vg;
	isVisible = false;
	captureAmount = 0.f;
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

	// Background color for the beacon
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
	nvgFill(vg);

    // Fill beacon bar with specific color depending on the hunter or monster
    float beaconWidth;
	NVGcolor barColor;

	// purple bar for monster
	if (captureAmount < 0) { 
		beaconWidth = w * captureAmount / MONSTER_BEACON_CAPTURE_THRESHOLD;
		barColor = nvgRGBA(243, 30, 243, 192); 

	// green bar for hunters
	} else { 
		beaconWidth = w * captureAmount / HUNTER_BEACON_CAPTURE_THRESHOLD;
		barColor = nvgRGBA(30, 243, 30, 192);
	}
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, beaconWidth, h, cornerRadius);
	nvgFillColor(vg, barColor);
	nvgFill(vg);
    
	nvgRestore(vg);
}

void BeaconBar::setVisible(bool visible) {
    this->isVisible = visible;
}