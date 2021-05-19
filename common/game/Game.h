#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "GridComponent.h"
#include "../networking/CommunicationConstants.h"
#include "../networking/GameState.h"
#include "Objective.h"
#include "Projectile.h"
#include "GameEvent.h"



class GamePlayer;
class Beacon;
typedef GamePlayer* PlayerList [MAX_PLAYERS]; // Set to MAX_PLAYERS just because Game assumes 4 players

class Game {

public:
    /* public member variables */
    Grids gameGrids; // Grids is a 2d array of GameGrid pointers
    PlayerList players; // PlayerList is an size-4-array of GamePlayer 
                        // (either Fighter type or Monster type for now)
    PlayerType idToJobType [MAX_PLAYERS]; // used for selecting purposes, -1 == not taken
    std::set<PlayerType> availableJobs;

    std::vector<GameEvent*> events;
    std::vector<GameUpdate> updates; // Buffers updates so it can be sent to clients
    std::vector<Objective *> objectives; // Keeps track of all the objectives in the game.
    std::unordered_map<int, Projectile*> projectiles; // Keeps track of all Projectile objects in the game.
    int nextProjectileId;
    Beacon* beacon; // only 1 beacon objective in the whole map and used to determine players in capture area

    int renderCount;
    bool started; // used to determine if we've passed the initial screen.

    // public member functions
    Game(); // default constructor
    void initPlayers(); // init playerList
    void initGameGrids(); // initialize gameGrids
    void initSelectScreenStructures(); // initialize the job class to player map
    void startSelectTimer(); // starts the timer for players to choose a class

    void cleanGameGrids(); 
    void cleanPlayers();
    void consumeObj(Objective *);

    GameState getGameState();
    static bool sameGameState (GameState s1, GameState s2);
    void printGameState (GameState gameState);
    ~Game(); // destructor

    bool handleInputs(CLIENT_INPUT playersInputs[PLAYER_NUM]);
    void handleUserClaim(CLIENT_INPUT claimType, int playerID);
    void updateProjectiles();
    void processEvent (GameEvent* event);
    void updateGameEvents();
    void updateBeacon();
    void checkEnd();

    void printGameGrids();
    void printPlayers();
    void printStats();
    void printSelectingScreen();

    // Network Optimization: GameUpdates
    void addUpdate(GameUpdate update);
    void clearUpdates();

    void handleUpdates(std::vector<GameUpdate> updates);
    void handleUpdate(GameUpdate update);

    void checkEvoLevel();
};




#endif