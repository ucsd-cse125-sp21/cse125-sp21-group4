#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include "GameComponent.h"

class GamePlayer;
typedef GamePlayer* PlayerList [PLAYER_NUM];

class Game {

public:
    /* public member variables */
    Grids gameGrids; // Grids is a 2d array of GameComponent pointers
    PlayerList players; // PlayerList is an size-4-array of gameComponent 
                        // (either Fighter type or Monster type)

    // public member functions
    Game(); // default constructor
    void initGameGrids(); // initialize gameGrids
    void cleanGameGrids(); 
    ~Game(); // destructor

    void swapGameComponents(GameComponent* a, GameComponent* b);
    GameComponent* getGameComponentInDirection (int x, 
                                            int y, Direction direction);
};




#endif