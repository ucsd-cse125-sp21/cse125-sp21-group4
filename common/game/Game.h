#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <vector>
#include "GridComponent.h"
#include "../constants.h"
#include "../GameState.h"

class GamePlayer;
typedef GamePlayer* PlayerList [PLAYER_NUM];

class Game {

public:
    /* public member variables */
    Grids gameGrids; // Grids is a 2d array of GameGrid pointers
    PlayerList players; // PlayerList is an size-4-array of GamePlayer 
                        // (either Fighter type or Monster type for now)

    std::vector<GameUpdate> updates;

    // public member functions
    Game(); // default constructor
    void initPlayers(); // init playerList
    void initGameGrids(); // initialize gameGrids
    void cleanGameGrids(); 
    void cleanPlayers();
    GameState getGameState();
    static bool sameGameState (GameState s1, GameState s2);
    void printGameState (GameState gameState);
    ~Game(); // destructor

    bool handleInputs(CLIENT_INPUT playersInputs[MAX_PLAYERS]);

    void printGameGrids();
    void printPlayers();

    void addUpdate(UPDATE_TYPE updateType, int id, int gridDeltaX, int gridDeltaY, float floatDeltaX, float floatDeltaY);
    void clearUpdates();
};




#endif