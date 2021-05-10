#include "SelectScreen.h"

SelectScreen::SelectScreen(NVGcontext* vg) {
    this->vg = vg;
	isVisible = false;
    isMonster = false;

}


void SelectScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 

    // If monster, display something else for the player to see.
    if(isMonster) {

        // Background color for the text
        nvgBeginPath(vg);
        nvgRoundedRect(vg, 0, 0, windowWidth, windowHeight, 3);
        nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
        nvgFill(vg);
        nvgClosePath(vg);

        // awaiting for other players text
        nvgFontSize(vg, windowHeight/32);
        nvgFontFace(vg, "sans-bold");
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
        nvgText(vg, windowWidth/2, windowHeight/2, "Waiting for Hunters to choose jobs...", NULL);
        

    // If not monster, display the select screen.
    } else {
        for(int i = 0; i < selectScreenElements.size(); i++) {
            selectScreenElements[i]->draw();
        }
    }
	nvgRestore(vg); 
}

void SelectScreen::setVisible(bool visible) {
    this->isVisible = visible;
}
void SelectScreen::setMonster(bool currentPlayerIsMonster) {
    this->isMonster = currentPlayerIsMonster;
}
void SelectScreen::handleRoleClaimed(PlayerType role) {
	switch(role) {
		case FIGHTER:
			selectScreenElements[1]->loadTexture("shaders/select_screen/fighter_selected.png");
			break;
		case MAGE:
			selectScreenElements[2]->loadTexture("shaders/select_screen/mage_selected.png");
			break;
		case CLERIC:
			selectScreenElements[3]->loadTexture("shaders/select_screen/cleric_selected.png");
			break;
		case ROGUE:
			selectScreenElements[4]->loadTexture("shaders/select_screen/rogue_selected.png");
			break;
	}
}

void SelectScreen::handleMouseSelect(float windowWidth, float windowHeight, int mouseX, int mouseY) {
    // TODO: handle mouse click/select
}