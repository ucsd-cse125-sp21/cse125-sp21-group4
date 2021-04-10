#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "../common/constants.h"
#include "../common/PlayerPosition.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>

class GameServer {
private:
    PlayerPosition playerPositions[MAX_PLAYERS];

    void processInput(int playerID, CLIENT_INPUT input); // Checks a single player input
    int checkBounds(int pos, int lower, int upper); // Checks the bounds of the movement to the map
public:
    GameServer(); // Constructs the server and threads required for 4 players

    std::vector<char> processInputs(std::vector<std::pair<int,CLIENT_INPUT>> inputs); // Processes the inputs and returns a game state



};

#endif