#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "CommunicationConstants.h"
#include "../game/GameConstants.h"
#include <vector>

struct GameActions {
    GAME_INPUT playersInputs[PLAYER_NUM];
};

struct GameState{
    PlayerPosition playersPosition[PLAYER_NUM];
    int playersHp[PLAYER_NUM];
};

#endif