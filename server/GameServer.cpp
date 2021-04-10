#include "GameServer.h"

GameServer::GameServer() {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        playerPositions[i].x = 0;
        playerPositions[i].y = 0;
        playerPositions[i].id = i;
    }
}

std::vector<char> GameServer::processInputs(std::vector<CLIENT_INPUT> inputs) {
    // For now, players can only input one action
    for(int i = 0; i < MAX_PLAYERS; i++) {
        processInput(i, inputs[i]);
    }

    // Prints out to the server console the states of the clients.
    for(int i = 0; i < MAX_PLAYERS; i++) {
        printf("Player %d is now on x: %d, y: %d\n", i, playerPositions[i].x,  playerPositions[i].y); 
    }

    std::vector<char> gameState;
    gameState.assign( (char *) &playerPositions, ((char*) &playerPositions) + sizeof(playerPositions));
    return gameState;
    
}

// Process the client's input
void GameServer::processInput(int assigned_player_id, CLIENT_INPUT input) {
    switch(input) {
        case MOVE_FORWARD:
            playerPositions[assigned_player_id].y++;
            break;
        case MOVE_LEFT:
            playerPositions[assigned_player_id].x--;
            break;
        case MOVE_BACKWARD:
            playerPositions[assigned_player_id].y--;
            break;
        case MOVE_RIGHT:
            playerPositions[assigned_player_id].x++;
            break;
    }

    // Check if x and y are in bounds
    playerPositions[assigned_player_id].y = checkBounds(playerPositions[assigned_player_id].y, MAP_LOWER_BOUND, MAP_HIGHER_BOUND);
    playerPositions[assigned_player_id].x = checkBounds(playerPositions[assigned_player_id].x, MAP_LOWER_BOUND, MAP_HIGHER_BOUND);
}

// Check Bounds of position and if outside of bounds, set it to the bound
int GameServer::checkBounds(int pos, int lower, int upper) {
    if(pos < lower) {
        return lower;
    }
    else if (pos > upper) {
        return upper;
    } 

    return pos;
}