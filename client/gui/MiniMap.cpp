#include "MiniMap.h"

MiniMap::MiniMap(NVGcontext* vg) {
    this->vg = vg;
	isVisible = false;

	currPlayerType = UNKNOWN;
	for(int i = 0; i < PLAYER_NUM; i++) {
		playerTypes[i] = UNKNOWN;
		playerPositions[i].x = SPAWN_POSITIONS[i][0];
		playerPositions[i].y = SPAWN_POSITIONS[i][1];
		pingPositions[i].x = SPAWN_POSITIONS[i][0];
		pingPositions[i].y = SPAWN_POSITIONS[i][1];
	}
	hasTeamCapturedBeacon = false;

	// Background image for the minimap
    image = nvgCreateImage(vg, "shaders/hud_elements/minimap.png", 0);
    nvgImageSize(vg, image, &imgWidth, &imgHeight);
}


void drawPlayerPixel(NVGcontext* vg, float x, float y, float w, float h, NVGcolor color) {
	nvgBeginPath(vg);
	nvgRect(vg, x, y, w, h);
	nvgFillColor(vg, color);
	nvgFill(vg);
	nvgClosePath(vg);
}

void MiniMap::draw(float x, float y, float w, float h) {

    // If not visible, don't draw it. If the player type is not known, don't draw it as well.
    if(!isVisible) {
        return;
    }

	// This will automatically return if the currentPlayerType is not known
	// SERVER_ENABLED flag because the currentPlayerType is UNKNOWN with zero server
	// but we still want to display the minimap for debugging
	#ifdef SERVER_ENABLED
	if (currPlayerType == UNKNOWN) {
		return;
	}
	#endif

	float cornerRadius = 3.0f;
	nvgSave(vg);

	// // Background color for the minimap
	// nvgBeginPath(vg);
	// nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	// nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
	// nvgFill(vg);
	// nvgClosePath(vg);


    NVGpaint imgPaint =  nvgImagePattern(vg, x, y, w, h, 0.0f/180.0f*NVG_PI, image, 1.f);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w, h, 5);
    nvgFillPaint(vg, imgPaint);
    nvgFill(vg);


	// draw safe region
	if (safeRegionRadius > 0) {
		nvgBeginPath(vg);
		nvgCircle(vg, x + safeRegionX/MAP_WIDTH*w, y + safeRegionY/MAP_HEIGHT*h, 
			safeRegionRadius/sqrt(pow(MAP_WIDTH,2)+pow(MAP_HEIGHT,2))*sqrt(pow(w,2)+pow(h,2)));
		nvgFillColor(vg, nvgRGBA(0,255,33,100));
		nvgFill(vg);
	}


	// Draw current player and teammates' positions
	if (currPlayerType == MONSTER)  {
		
		// monsters should only display themselves
		float playerX = playerPositions[currPlayerId].x / MAP_WIDTH * w;
		float playerY = playerPositions[currPlayerId].y / MAP_HEIGHT * h;
		drawPlayerPixel(vg, x + playerX - 1, y + playerY - 1, 3, 3, nvgRGBA(0,255,0,255));
	} else {

		for(int i = 0; i < PLAYER_NUM; i++) {
			if(playerTypes[i] != MONSTER) {

				// display all hunter teammates
				float playerX = playerPositions[i].x / MAP_WIDTH * w;
				float playerY = playerPositions[i].y / MAP_HEIGHT * h;
				drawPlayerPixel(vg, x + playerX - 1, y + playerY - 1, 3, 3, nvgRGBA(0,255,0,255));
			}
		}

	}

	// Draw enemy team's positions, depends if beacon is enabled for this team.
	if(hasTeamCapturedBeacon) {
		
		for(int i = 0; i < PLAYER_NUM; i++) {

			// For monsters, display all 3 positions
			if(playerTypes[i] != currPlayerType && currPlayerType == MONSTER) {
				float playerX = pingPositions[i].x / MAP_WIDTH * w;
				float playerY = pingPositions[i].y / MAP_HEIGHT * h;
				drawPlayerPixel(vg, x + playerX - 1, y + playerY - 1, 3, 3, nvgRGBA(255,0,0,255));
			} 

			// For hunters, only display the monster
			else if(playerTypes[i] != currPlayerType && playerTypes[i] == MONSTER) {
				float playerX = pingPositions[i].x / MAP_WIDTH * w;
				float playerY = pingPositions[i].y / MAP_HEIGHT * h;
				drawPlayerPixel(vg, x + playerX - 1, y + playerY - 1, 3, 3, nvgRGBA(255,0,0,255));
			} 
		}
	}

	nvgRestore(vg);
}

void MiniMap::setVisible(bool visible) {
    this->isVisible = visible;
}
void MiniMap::setCurrentPlayer(int id, PlayerType type) {
	currPlayerId = id;
	currPlayerType = type;
	playerTypes[id] = type;
}
void MiniMap::setPlayerType(int id, PlayerType type) {
	playerTypes[id] = type;
}
void MiniMap::updatePlayerPosition(int id, PlayerPosition pos) {
	playerPositions[id] = pos;
}
void MiniMap::updatePlayerPositionDelta(int id, float deltaX, float deltaY) {
	playerPositions[id].x += deltaX;
	playerPositions[id].y += deltaY;
}
void MiniMap::handleCaptureEvent(float captureAmount) {
	if((captureAmount <= MONSTER_BEACON_CAPTURE_THRESHOLD && currPlayerType == MONSTER) || 
	   (captureAmount >= HUNTER_BEACON_CAPTURE_THRESHOLD && currPlayerType != MONSTER)) {
		hasTeamCapturedBeacon = true;
	}
}
void MiniMap::updatePingPosition(int id, PlayerPosition pos) {
	pingPositions[id] = pos;
}

void MiniMap::reset() {
	isVisible = false;

	currPlayerType = UNKNOWN;
	for(int i = 0; i < PLAYER_NUM; i++) {
		playerTypes[i] = UNKNOWN;
		playerPositions[i].x = SPAWN_POSITIONS[i][0];
		playerPositions[i].y = SPAWN_POSITIONS[i][1];
		pingPositions[i].x = SPAWN_POSITIONS[i][0];
		pingPositions[i].y = SPAWN_POSITIONS[i][1];
	}
	hasTeamCapturedBeacon = false;
}