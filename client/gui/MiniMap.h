#ifndef _GUI_MINIMAP_H_
#define _GUI_MINIMAP_H_

#include "../Core.h" // GL required for the NanoGUI.
#include "../nanovg/nanovg.h"
#include "../../common/game/GameConstants.h"
#include <string>

class MiniMap {

public:
    int currPlayerId;
    int image,  imgWidth, imgHeight;
    PlayerType currPlayerType; // used to determine if this player should see teammates
    PlayerType playerTypes[PLAYER_NUM]; // used later if we want to add minimap icons
    PlayerPosition playerPositions[PLAYER_NUM]; // actual player positions on the map
    PlayerPosition pingPositions[PLAYER_NUM];
    NVGcontext* vg;
    bool isVisible;
    bool hasTeamCapturedBeacon; // for those who have captured the beacon


    MiniMap(NVGcontext* vg);
    void draw(float x, float y, float w, float h);
    void setVisible(bool visible);
    void setCurrentPlayer(int, PlayerType);
    void setPlayerType(int, PlayerType);
    void updatePlayerPosition(int, PlayerPosition);
    void updatePlayerPositionDelta(int, float deltaX, float deltaY);
    void updatePingPosition(int, PlayerPosition);
    void handleCaptureEvent(float);
    void reset();
};
#endif