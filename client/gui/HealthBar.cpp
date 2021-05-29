#include "HealthBar.h"

HealthBar::HealthBar(NVGcontext* vg) {
    this->vg = vg;
    maxHp = 144;
    hp = 190;
	shadowHp = 123;
	isVisible = false;
	barColor = redColor; // default red
	shadowColor = shadowRedColor;

    this->image = nvgCreateImage(vg, "shaders/hud_elements/health_bar.png", 0);
	// printf("image: %d\n", image);
    nvgImageSize(vg, image, &imgWidth, &imgHeight);
}

void HealthBar::setHp(int hp) {
    this->hp = hp;
	this->shadowHp = hp;
}

void HealthBar::decrementHp(int damage) {
	this->hp -= damage;
}
void HealthBar::incrementHp(int heal) {
	this->hp += heal;
}
void HealthBar::setMaxHp(int newMax) {
    this->maxHp = newMax;
}

void HealthBar::initGivenPlayerType(PlayerType type) {
	switch(type) {
		case FIGHTER:
			setHp(FIGHTER_MAX_HP);
			setMaxHp(FIGHTER_MAX_HP);
			break;
		case MAGE:
			setHp(MAGE_MAX_HP);
			setMaxHp(MAGE_MAX_HP);
			break;
		case CLERIC:
			setHp(CLERIC_MAX_HP);
			setMaxHp(CLERIC_MAX_HP);
			break;
		case ROGUE:
			setHp(ROGUE_MAX_HP);
			setMaxHp(ROGUE_MAX_HP);
			break;
		case MONSTER:
			setHp(MONSTER_MAX_HP);
			setMaxHp(MONSTER_MAX_HP);
	}
}

void HealthBar::draw(float x, float y, float w, float h) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	float cornerRadius = 3.0f;
	nvgSave(vg);

	// Background image for the hp bar
    NVGpaint imgPaint =  nvgImagePattern(vg, x, y, w, h, 0.0f/180.0f*NVG_PI, this->image, HEALTH_BAR_ALPHA / 255.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w, h, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);


	// // Background color for the hp bar
	// nvgBeginPath(vg);
	// nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	// nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
	// nvgFill(vg);

	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> durationSinceFlash = now - lastFlash;
	auto durationInMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceFlash).count();

	// If now < flashUntil && it's been > INTERVAL since last Flash, switch colors
	if(now < flashUntil && durationInMilliSeconds > HPBAR_DAMAGE_TAKEN_FLASHING_INTERVAL_MS) {

		// Sufficient to just check one datafield because health bar can only be two colors
		if(barColor.r == redColor.r) {
			barColor = whiteColor;
			shadowColor = whiteColor;
		} else {
			barColor = redColor;
			shadowColor = shadowRedColor;
		}
		lastFlash = now;

	// else if now >= flashUntil, return back to red color.
	} else if (now >= flashUntil) {
		barColor = redColor;
		shadowColor = shadowRedColor;
	}

	// If actualBarHp is not equal to hp, increment or decrement it towards the hp
	std::chrono::duration<float> durationSinceShadowChange = now - lastShadowChange;
	auto shadowDurationInMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceShadowChange).count();

	if(shadowDurationInMilliSeconds > HPBAR_SHADOW_HP_INTERVAL_MS) {
		if (shadowHp > hp) {
			shadowHp--;
		} else if (shadowHp < hp) {
			shadowHp++;
		}
		lastShadowChange = now;
	}


	// Draw the bar within the hud element
	float barHeight = h / 5;
	float barWidth = w * 4.75 / 6;
	float barX = x + w / 6.3;
	float barY = y + h / 3.1;

    // shadow hp bar
    float shadowHpWidth = std::min(barWidth * (float) shadowHp / (float) maxHp, barWidth);
	nvgBeginPath(vg);
	nvgRoundedRect(vg, barX, barY, shadowHpWidth, barHeight, cornerRadius);
	nvgFillColor(vg, shadowColor);
	nvgFill(vg);

	// actual hp bar (only fill if red)
	if(barColor.r == redColor.r) {
		float hpWidth = std::min(barWidth * (float) hp / (float) maxHp, barWidth);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, barX, barY, hpWidth, barHeight, cornerRadius);
		nvgFillColor(vg, barColor);
		nvgFill(vg);

		// if hp > maxHP, show armored bar
		if(hp > maxHp) {
			float armorWidth = (float) (hp - maxHp) / (float) maxHp * barWidth;
			nvgBeginPath(vg);
			nvgRoundedRect(vg, barX, barY + barHeight + barHeight / 10, armorWidth, barHeight / 4, cornerRadius);
			nvgFillColor(vg, armorColor);
			nvgFill(vg);
		}
	}




    // hp text
	nvgFontSize(vg, barHeight / 1.2);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

	nvgFontBlur(vg, 0);
	nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
	std::string hpText = "HP : " + std::to_string(hp) + "/" + std::to_string(maxHp);
	nvgText(vg, barX + 5, barY + barHeight / 2, hpText.c_str(), NULL);
    

	nvgRestore(vg);
}

void HealthBar::setVisible(bool visible) {
    this->isVisible = visible;
}

void HealthBar::flashHealthBar() {
	flashUntil = std::chrono::steady_clock::now() + std::chrono::milliseconds(HPBAR_DAMAGE_TAKEN_FLASHING_TIME_MS);
}