#include "GUIManager.h"

#include "../nanovg/nanovg.h"
#ifndef NANOVG_GL3_IMPLEMENTATION
#define NANOVG_GL3_IMPLEMENTATION
#endif
#include "../nanovg/nanovg_gl.h"

GUIManager::GUIManager(int width, int height, int fbWidth, int fbHeight) {

    this->windowWidth = width;
    this->windowHeight = height;
    this->fbWidth = fbWidth;
    this->fbHeight = fbHeight;
    this->pixelRatio = (float)this->fbWidth / (float)this->windowWidth;

	// Initialize NanoVG
	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if(vg == NULL) {
		printf("Could not init nanovg.\n");
	}
	nvgCreateFont(vg, "sans-bold", "./nanovg/Roboto-Bold.ttf");
	nvgCreateFont(vg, "sans", "./nanovg/Roboto-Regular.ttf");
	nvgCreateFont(vg, "sans-light", "./nanovg/Roboto-Light.ttf");

	healthBar = new HealthBar(vg);
	beaconBar = new BeaconBar(vg);
	evoBar = new EvolutionBar(vg);
	miniMap = new MiniMap(vg);
	selectScreen = new SelectScreen(vg);
	connectingScreen = new ConnectingScreen(vg);
	endScreen = new EndScreen(vg);
	splashScreen = new SplashScreen(vg);
	
	// Will be initialized later
	for (int i = 0; i < NUM_COOLDOWNS; i++) {
		cooldownIcons[i] = nullptr;
	}

}

void GUIManager::draw() {
	nvgBeginFrame(vg, this->windowWidth, this->windowHeight, this->pixelRatio);
	// starter code window: this->drawWindow("Test", 50, 50, this->windowWidth / 4, this->windowHeight / 4);
	healthBar->draw(30, 14.5 * this->windowHeight / 16, this->windowWidth / 2.5, this->windowHeight / 16);
	beaconBar->draw(this->windowWidth / 2 - (this->windowWidth / 2.5) / 2, this->windowHeight / 16, this->windowWidth / 2.5, this->windowHeight / 16);
	evoBar->draw(30, 13.5 * this->windowHeight / 16, this->windowWidth / 2.5, this->windowHeight / 16);
	for(int i = 0; i < NUM_COOLDOWNS; i++) {
		if(cooldownIcons[i] != nullptr) {
			cooldownIcons[i]->draw(this->windowWidth / 14 * i + this->windowWidth / 2, 13.5 * this->windowHeight / 16, this->windowWidth / 16, this->windowWidth / 16);
		}
	}
	miniMap->draw(this->windowWidth - (MAP_WIDTH / 2), this->windowHeight - (MAP_HEIGHT / 2), (MAP_WIDTH / 2), (MAP_HEIGHT / 2));
	selectScreen->draw(this->windowWidth, this->windowHeight);
	splashScreen->draw(this->windowWidth, this->windowHeight);
	connectingScreen->draw(this->windowWidth, this->windowHeight);
	endScreen->draw(this->windowWidth, this->windowHeight);

	nvgEndFrame(vg);
}


void GUIManager::resizeWindow(int width, int height, int fbWidth, int fbHeight) {
    this->windowWidth = width;
    this->windowHeight = height;
    this->fbWidth = fbWidth;
    this->fbHeight = fbHeight;
    this->pixelRatio = (float)this->fbWidth / (float)this->windowWidth;
}


void GUIManager::setHUDVisible(bool visibility) {
	healthBar->setVisible(visibility);
	beaconBar->setVisible(visibility);
	miniMap->setVisible(visibility);
	evoBar->setVisible(visibility);

	for(int i = 0; i < NUM_COOLDOWNS; i++) {
		if(cooldownIcons[i] != nullptr) {
			cooldownIcons[i]->setVisible(visibility);
		}
	}

}

void GUIManager::setSelectScreenVisible(bool visibility) {
	selectScreen->setVisible(visibility);
}

void GUIManager::setConnectingScreenVisible(bool visibility) {
	connectingScreen->setVisible(visibility);
}

void GUIManager::setGameEndVisible(bool visibility) {
	endScreen->setVisible(visibility);
}

void GUIManager::setSplashScreenVisible(bool visibility) {
	splashScreen->setVisible(visibility);
}


void GUIManager::handleMouseSelect(int x, int y) {
	// might need this if we decide to add mouse support.
}
void GUIManager::drawCenterText(std::string text, int windowWidth, int windowHeight) {
	nvgSave(vg);


	// Instruction to player.
	nvgFontSize(vg, windowHeight/32);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

	nvgFontBlur(vg, 0);
	nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
	nvgText(vg, windowWidth/2, windowHeight/4, text.c_str(), NULL);


	nvgRestore(vg);
}


// Example NanoVG method from https://github.com/memononen/nanovg. We can delete this later.
// Right now I'm using it as example code to draw new elements
void GUIManager::drawWindow(const char* title, float x, float y, float w, float h)
{
	float cornerRadius = 3.0f;
	NVGpaint shadowPaint;
	NVGpaint headerPaint;

	nvgSave(vg);
	//	nvgClearState(vg);

	// Window
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
	//	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgFill(vg);

	// Drop shadow
	shadowPaint = nvgBoxGradient(vg, x, y + 2, w, h, cornerRadius * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
	nvgBeginPath(vg);
	nvgRect(vg, x - 10, y - 10, w + 20, h + 30);
	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);

	// Header
	headerPaint = nvgLinearGradient(vg, x, y, x, y + 15, nvgRGBA(255, 255, 255, 8), nvgRGBA(0, 0, 0, 16));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x + 1, y + 1, w - 2, 30, cornerRadius - 1);
	nvgFillPaint(vg, headerPaint);
	nvgFill(vg);
	nvgBeginPath(vg);
	nvgMoveTo(vg, x + 0.5f, y + 0.5f + 30);
	nvgLineTo(vg, x + 0.5f + w - 1, y + 0.5f + 30);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
	nvgStroke(vg);

	nvgFontSize(vg, 15.0f);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

	nvgFontBlur(vg, 2);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
	nvgText(vg, x + w / 2, y + 16 + 1, title, NULL);

	nvgFontBlur(vg, 0);
	nvgFillColor(vg, nvgRGBA(220, 220, 220, 160));
	nvgText(vg, x + w / 2, y + 16, title, NULL);

	nvgRestore(vg);
}

void GUIManager::updateHUDPositions() {

	// Select Screen is part of "HUD"
	selectScreen->updatePositions();

	// Other HUD elemeents...
}
void GUIManager::reset() {
	// Elements to reset:
	// healthBar --> not needed because values update during character selection
	// beaconBar
	beaconBar->setAmount(0.f);
	// evoBar
	evoBar->setEvo(0.f);
	// miniMap
	miniMap->reset();
	// selectScreen
	selectScreen->reset();
	// connectingScreen
	connectingScreen->init();	
	// cooldown icons
	for(int i = 0; i < NUM_COOLDOWNS; i++) {
		if(cooldownIcons[i] != nullptr) {
			delete cooldownIcons[i];
			cooldownIcons[i] = nullptr;
		}
	}
}

void GUIManager::setSplashLoaded(bool loaded) {
	splashScreen->setSplashLoaded(loaded);
}

void GUIManager::initCooldownIcons(PlayerType type) {
	switch(type) {
		case FIGHTER:
			cooldownIcons[NORMAL_ATTACK] = new CooldownIcon(vg, "Slash", FIGHTER, FIGHTER_ATTACK_TIME_INTERVAL);
			break;
		case MAGE:
			cooldownIcons[NORMAL_ATTACK] = new CooldownIcon(vg, "Fire Arrow", MAGE, MAGE_ATTACK_TIME_INTERVAL);
			cooldownIcons[UNIQUE_ATTACK] = new CooldownIcon(vg, "Fire Ball", MAGE, FIREBALL_TIME_INTERVAL);
			break;
		case CLERIC:
			cooldownIcons[NORMAL_ATTACK] = new CooldownIcon(vg, "Light Ball", CLERIC, CLERIC_ATTACK_TIME_INTERVAL);
			cooldownIcons[UNIQUE_ATTACK] = new CooldownIcon(vg, "Area Heal", CLERIC, HEALING_AURA_TIME_INTERVAL);
			break;
		case ROGUE:
			cooldownIcons[NORMAL_ATTACK] = new CooldownIcon(vg, "Arrow", ROGUE, ROGUE_ATTACK_TIME_INTERVAL);
			break;
		case MONSTER:
			cooldownIcons[NORMAL_ATTACK] = new CooldownIcon(vg, "Throw", MONSTER, MONSTER_ATTACK_TIME_INTERVAL);
			cooldownIcons[UNIQUE_ATTACK] = new CooldownIcon(vg, "Claw", MONSTER, MONSTER_ATTACK_TIME_INTERVAL);
			break;
		default:
			printf("Error in init cooldown icons\n");
			break;
	}
}

// Window.cpp should have already checked this is the right player id.
void GUIManager::handleCooldownUpdate(CooldownID id) {
	if(cooldownIcons[id] != nullptr) {
		cooldownIcons[id]->handleAction();
	}
}