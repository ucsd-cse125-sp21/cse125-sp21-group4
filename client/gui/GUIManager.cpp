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
	miniMap = new MiniMap(vg);
	selectScreen = new SelectScreen(vg);
}

void GUIManager::draw() {
	nvgBeginFrame(vg, this->windowWidth, this->windowHeight, this->pixelRatio);
	// starter code window: this->drawWindow("Test", 50, 50, this->windowWidth / 4, this->windowHeight / 4);
	healthBar->draw(30, 14 * this->windowHeight / 16, this->windowWidth / 2.5, this->windowHeight / 16);
	beaconBar->draw(30, this->windowHeight / 16, this->windowWidth - 60, this->windowHeight / 64);
	miniMap->draw(this->windowWidth - (MAP_WIDTH / 3), this->windowHeight - (MAP_HEIGHT / 3), (MAP_WIDTH / 3), (MAP_HEIGHT / 3));

	selectScreen->draw(this->windowWidth, this->windowHeight);

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
}

void GUIManager::setSelectScreenVisible(bool visibility) {
	selectScreen->setVisible(visibility);
}

void GUIManager::handleMouseSelect(int x, int y) {
	selectScreen->handleMouseSelect(this->windowWidth, this->windowHeight, x, y);
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
