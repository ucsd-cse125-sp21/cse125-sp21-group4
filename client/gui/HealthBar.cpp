#include "HealthBar.h"

HealthBar::HealthBar(NVGcontext* vg) {
    this->vg = vg;
    maxHp = 144;
    hp = 123;
	isVisible = false;
}

void HealthBar::setHp(int hp) {
    this->hp = hp;
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

	// Background color for the hp bar
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
	nvgFill(vg);

    // hp bar
    float hpWidth = w * (float) hp / (float) maxHp;
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, hpWidth, h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(243, 30, 30, 192));
	nvgFill(vg);


    // hp text
	nvgFontSize(vg, h/2);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

	nvgFontBlur(vg, 0);
	nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
	std::string hpText = "HP : " + std::to_string(hp) + "/" + std::to_string(maxHp);
	nvgText(vg, x + 5, y + h / 2, hpText.c_str(), NULL);
    

	nvgRestore(vg);
}

void HealthBar::setVisible(bool visible) {
    this->isVisible = visible;
}