#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "constants.h"
#include <vector>

struct GameState{
    // std::vector<std::pair<int,CLIENT_INPUT>> inputs;
    CLIENT_INPUT playersInputs[MAX_PLAYERS];
    
};

#endif