#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "PlayerPosition.h"
#include "constants.h"
struct GameState{
    PlayerPosition playerPositions[MAX_PLAYERS];
};
#endif