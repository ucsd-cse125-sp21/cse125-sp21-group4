#include "SelectScreen.h"

SelectScreen::SelectScreen(NVGcontext* vg) {
    this->vg = vg;
	isVisible = false;
    isMonster = false;
    hasClaimed = false;

    claimed[FIGHTER] = false;
    claimed[MAGE] = false;
    claimed[CLERIC] = false;
    claimed[ROGUE] = false;
    selecting = UNKNOWN;

    // ScreenElements are initialized by Window.cpp.
}


void SelectScreen::draw(float windowWidth, float windowHeight) {

    // If not visible, don't draw it.
    if(!isVisible) {
        return;
    }

	nvgSave(vg); 

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = now - timeSelectStarted;
    int numSecsLeft = SELECT_SCREEN_TIME - std::chrono::duration_cast<std::chrono::seconds>(duration).count();
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
        std::string text = "Waiting for Hunters to choose jobs (Countdown: " + std::to_string(numSecsLeft) + ").";
        nvgText(vg, windowWidth/2, windowHeight/2, text.c_str(), NULL);
        

    // If not monster, display the select screen.
    } else {
        for(int i = 0; i < selectScreenElements.size(); i++) {
            selectScreenElements[i]->draw();
        }

        // Draw instructions to tell player how to choose a role.
        // awaiting for other players text
        nvgFontSize(vg, windowHeight/48);
        nvgFontFace(vg, "sans-bold");
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
        std::string text = "Press 1-4 to select a role. Press Enter to claim your role (" +    
            std::to_string(numSecsLeft) + " seconds remaining).";
        nvgText(vg, windowWidth/2, windowHeight - windowHeight/24, text.c_str() , NULL);

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
    claimed[role] = true;
}

void SelectScreen::handleSelecting(PlayerType roleSelecting) {

    // If it's already claimed, do not allow texture change.
    if(claimed[roleSelecting] || hasClaimed) {
        return;
    }

    // reset old selecting texture to unselected.
    switch (selecting) {
        case FIGHTER:
			selectScreenElements[1]->loadTexture("shaders/select_screen/fighter_unselected.png");
            break;
        case MAGE:
			selectScreenElements[2]->loadTexture("shaders/select_screen/mage_unselected.png");
            break;
        case CLERIC:
			selectScreenElements[3]->loadTexture("shaders/select_screen/cleric_unselected.png");
            break;
        case ROGUE:
			selectScreenElements[4]->loadTexture("shaders/select_screen/rogue_unselected.png");
            break;
    }

    selecting = roleSelecting;

    // selecting on new button
    switch (roleSelecting) {
        case FIGHTER:
			selectScreenElements[1]->loadTexture("shaders/select_screen/fighter_selecting.png");
            break;
        case MAGE:
			selectScreenElements[2]->loadTexture("shaders/select_screen/mage_selecting.png");
            break;
        case CLERIC:
			selectScreenElements[3]->loadTexture("shaders/select_screen/cleric_selecting.png");
            break;
        case ROGUE:
			selectScreenElements[4]->loadTexture("shaders/select_screen/rogue_selecting.png");
            break;
    }
}


void SelectScreen::startTimer(std::chrono::steady_clock::time_point startTime) {
    // Commented out buggy code for now, the issue was that steady_clock does not work across machines
    // timeSelectStarted = startTime;
    
    // There **should** be no delay between when the graphics client sees SelectScreen and server's update.
    timeSelectStarted = std::chrono::steady_clock::now();
}

void SelectScreen::updatePositions() {
	for (int i = 0; i < selectScreenElements.size(); i++) {
		if (selectScreenElements[i] != nullptr) {
			selectScreenElements[i]->update();
		}
	}
}