#include "CooldownIcon.h"

CooldownIcon::CooldownIcon(NVGcontext* vg, std::string actionFilePath, PlayerType type, unsigned int cooldown_ms) {
    this->vg = vg;
	this->lastActionTimePoint = std::chrono::steady_clock::now();
	this->actionReadyTimePoint = std::chrono::steady_clock::now();

	this->playerType = type;
	this->cooldownInMilliseconds = cooldown_ms;

	this->isVisible = false;

    this->image = nvgCreateImage(vg, actionFilePath.c_str(), NVG_IMAGE_FLIPY);
    nvgImageSize(vg, this->image, &imgWidth, &imgHeight);
}

void CooldownIcon::draw(float x, float y, float w, float h) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	float cornerRadius = 3.0f;
	nvgSave(vg);
    
	// Draw background for the cooldown icon
	nvgBeginPath(vg);

	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgFillColor(vg, shadowColor);
	nvgFill(vg);
	
    NVGpaint imgPaint =  nvgImagePattern(vg, x, y, w, h, 0.0f/180.0f*NVG_PI, this->image, 1.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg,  x, y, w, h, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::duration<float> duration = actionReadyTimePoint - now;
	long long millisecondsLeft = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	float cooldownHeight = h * ((float) millisecondsLeft / (float) cooldownInMilliseconds);

	// Draw darker bar for cooldown
	if(cooldownHeight > 0) {
		nvgBeginPath(vg);
		nvgRoundedRect(vg, x, y + (h - cooldownHeight), w, cooldownHeight, cornerRadius);
		nvgFillColor(vg, darkerShadowColor);
		nvgFill(vg);
	}

	// Draw text for cooldown. (First line = action name; second line = cooldown)
	float millisecondsUntilCooldown = 0;
	if(millisecondsLeft <= 0) {
		millisecondsUntilCooldown = 0;
	} else {
		millisecondsUntilCooldown = ((float) millisecondsLeft) / 1000.0;
	}
	std::string cooldownText = std::to_string(millisecondsUntilCooldown).substr(0, 3) + "s";
	nvgFontSize(vg, h / 6.5);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    
	// nvgFontBlur(vg, 0);
	// nvgFillColor(vg, lightColor);
	// nvgText(vg, x + w/2, y + h/3, actionName.c_str(), NULL);

	nvgFontBlur(vg, 0);
	nvgFillColor(vg, lightColor);
	nvgText(vg, x + w/2, y + 4 * h/5, cooldownText.c_str(), NULL);

	nvgRestore(vg);
}

void CooldownIcon::setVisible(bool visible) {
    this->isVisible = visible;
}

void CooldownIcon::handleAction() {
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    actionReadyTimePoint = now + std::chrono::milliseconds(cooldownInMilliseconds);
}