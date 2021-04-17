#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "constants.h"
#include "game/GameConstants.h"
#include <vector>

struct GameActions {
    CLIENT_INPUT playersInputs[MAX_PLAYERS];
};

struct GameState{
    PlayerPosition playersPosition[MAX_PLAYERS];
    int playersHp[MAX_PLAYERS];
};

#endif