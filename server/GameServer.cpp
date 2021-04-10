#include "GameServer.h"

GameServer::GameServer() {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        gameState.playerPositions[i].x = 0;
        gameState.playerPositions[i].y = 0;
        gameState.playerPositions[i].id = i;
    }
}

std::vector<char> GameServer::processInputs(std::vector<std::pair<int,CLIENT_INPUT>> inputs) {
    // For now, players can only input one action
    auto iter = inputs.begin();
    while(iter != inputs.end()) {
        std::pair<int, CLIENT_INPUT> input = *iter;
        processInput(input.first, input.second);
        iter++;
    }

    // Prints out to the server console the states of the clients.
    for(int i = 0; i < MAX_PLAYERS; i++) {
        printf("Player %d is now on x: %d, y: %d\n", i, gameState.playerPositions[i].x,  gameState.playerPositions[i].y); 
    }

    // TODO: Currently, we just make the gamestate out of the playerpositions, we can change thing later to be only things that change. OR, have more things in the gamestate to send over. 
    // For example: we could have variable things in the gamestate, so we will need a variable length protocol. 
    std::vector<char> gameStateSerialized;
    gameStateSerialized.assign( (char *) &gameState, ((char*) &gameState) + sizeof(gameState));
    return gameStateSerialized;
    
}

// Process the client's input
void GameServer::processInput(int assigned_player_id, CLIENT_INPUT input) {
    switch(input) {
        case MOVE_FORWARD:
            gameState.playerPositions[assigned_player_id].y++;
            break;
        case MOVE_LEFT:
            gameState.playerPositions[assigned_player_id].x--;
            break;
        case MOVE_BACKWARD:
            gameState.playerPositions[assigned_player_id].y--;
            break;
        case MOVE_RIGHT:
            gameState.playerPositions[assigned_player_id].x++;
            break;
    }

    // Check if x and y are in bounds
    gameState.playerPositions[assigned_player_id].y = checkBounds(gameState.playerPositions[assigned_player_id].y, MAP_LOWER_BOUND, MAP_HIGHER_BOUND);
    gameState.playerPositions[assigned_player_id].x = checkBounds(gameState.playerPositions[assigned_player_id].x, MAP_LOWER_BOUND, MAP_HIGHER_BOUND);
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