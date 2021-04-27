#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include "GridComponent.h"
#include "../networking/CommunicationConstants.h"
#include "../networking/GameState.h"
#include "Objective.h"
#include "Projectile.h"



class GamePlayer;
class Beacon;
typedef GamePlayer* PlayerList [MAX_PLAYERS]; // Set to MAX_PLAYERS just because Game assumes 4 players

class Game {

public:
    /* public member variables */
    Grids gameGrids; // Grids is a 2d array of GameGrid pointers
    PlayerList players; // PlayerList is an size-4-array of GamePlayer 
                        // (either Fighter type or Monster type for now)

    std::vector<GameUpdate> updates; // Buffers updates so it can be sent to clients
    std::vector<Objective *> objectives; // Keeps track of all the objectives in the game.
    std::vector<Projectile*> projectiles; // Keeps track of all Projectile objects in the game.
    Beacon* beacon; // only 1 beacon objective in the whole map and used to determine players in capture area

    // public member functions
    Game(); // default constructor
    void initPlayers(); // init playerList
    void initGameGrids(); // initialize gameGrids

    void cleanGameGrids(); 
    void cleanPlayers();
    void consumeObj(Objective *);

    GameState getGameState();
    static bool sameGameState (GameState s1, GameState s2);
    void printGameState (GameState gameState);
    ~Game(); // destructor

    bool handleInputs(CLIENT_INPUT playersInputs[PLAYER_NUM]);
    void updateProjectiles();
    void updateBeacon();

    void printGameGrids();
    void printPlayers();
    void printStats();

    // Network Optimization: GameUpdates
    void addUpdate(GameUpdate update);
    void clearUpdates();

    void handleUpdates(std::vector<GameUpdate> updates);
    void handleUpdate(GameUpdate update);
};




#endif