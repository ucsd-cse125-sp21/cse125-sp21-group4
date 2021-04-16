#include "Game.h"
#include "Obstacle.h"
#include "Monster.h"
#include "Fighter.h"
#include "Space.h"

#define MONSTER_SYMBOL 'M'
#define FIGHTER_SYMBOL 'F'
#define OBSTACLE_SYMBOL '+'
#define SPACE_SYMBOL ' '

using namespace std;

Game::Game() {
    initGameGrids();
}



/* return true if (x, y) is the boundary */
bool isBoundary (int x, int y) {
    if (x == 0) return true; // upper boundary
    if (y == 0) return true; // left boudary
    if (x == MAP_HEIGHT - 1) return true; // bottom boundary
    if (y == MAP_WIDTH - 1) return true; // right boundary

    return false;
}

/* 
    Return true if (x, y) is one of the four spawn regions.
    In current game settings, spawn regions are set to four corners of the map
 */
bool isSpawnRegion (int x, int y) {
    if (x == 1 && y == 1) return true; // upper left spawn region
    if (x == 1 && y == MAP_WIDTH - 2) return true; // upper right spawn region
    if (x == MAP_HEIGHT - 2 && y == 1) return true; // bottom left spawan region
    if (x == MAP_HEIGHT - 2 && y == MAP_WIDTH - 2) return true; // bottom right 
                                                                // spawn region

    return false;
}

/*
    Initialize gameGrids by newing a corresponding GameComponent at every 
    position.

    Current Settings:
    - Edges are occupied by OBSTACLE
    - Four players are placed at four corners
        - First 3 players in list will be fighters
        - The 4th player will be the monster
    - Every where else will be SPACE by now

    Looks like
    +++++++++++++
    +F         F+
    +           +
    +           +
    +F         M+
    +++++++++++++
*/
void Game::initGameGrids() {
    int currPlayerIndex  =  0;
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {

            ComponentPosition position;
            position.x = i;
            position.y = j;
            // put obstacles on the boundary
            if (isBoundary(i, j))
                gameGrids[i][j] = new Obstacle(position);

            // 4 players are assigned as 3 fighters and 1 monster
            else if (isSpawnRegion(i, j)) {
                // last player in list will be the monster
                if (currPlayerIndex == 3) {
                    gameGrids[i][j] = new Monster(position);
                } else {
                    gameGrids[i][j] = new Fighter(position);
                }
                players[currPlayerIndex++] = (GamePlayer*) gameGrids[i][j];
            }

            else
                gameGrids[i][j] = new Space(position);
        }
    }
}

void Game::cleanGameGrids() {
    // delete all the GameComponents related to the game grids
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            if (!gameGrids[i][j]) delete gameGrids[i][j];
            gameGrids[i][j] = NULL;
        }
    }

    // avoid dangling pointers for players
    for (int i = 0; i < PLAYER_NUM; i++){
        players[i] = NULL;
    }   
}

Game::~Game() {
    cleanGameGrids();
}


bool Game::handleInputs(CLIENT_INPUT playersInputs[MAX_PLAYERS]) {
    bool flag = false; // flag is true if there is effective movement in this round
                       // this is used to test on console, this can be removed later
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (playersInputs[i] != NO_MOVE) flag = true;
        players[i]->handleUserInput(this, playersInputs[i]);
    }
    return flag;
}

void printComponent (GameComponent* grid) {
    if (grid->isMonster()) cout << MONSTER_SYMBOL;
    else if (grid->isFighter()) cout << FIGHTER_SYMBOL;
    else if (grid->isSpace()) cout << SPACE_SYMBOL;
    else if (grid->isObstacle()) cout << OBSTACLE_SYMBOL;
    else cout << "Encounter Unknown Type: " << grid->getType() << endl;
}


void Game::printGameGrids () {
    cout << "======= print the board ======" << endl;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            GameComponent* grid = gameGrids[j][i];

            printComponent(grid);

        }
        cout << "\n";

    }
}

void Game::printPlayers () {
    cout << " ======== print the players ======= " << endl;
    for (int i = 0; i < PLAYER_NUM; i++) {
        cout << "Player " << i << ": ";
        printComponent(players[i]);
        cout << ", HP: " << players[i]->getHp();
        cout << endl;
    }
}


/*
    Swap two GameComponent pointers in gameGrids, and change the two 
    GameComponent's position accordingly
*/
void Game::swapGameComponents(GameComponent* a, GameComponent* b) {
    int axOld = a->getPosition().x;
    int ayOld = a->getPosition().y;
    int bxOld = b->getPosition().x;
    int byOld = b->getPosition().y;

    gameGrids[axOld][ayOld] = b;
    gameGrids[bxOld][byOld] = a;

    a->setPosition(bxOld, byOld);
    b->setPosition(axOld, ayOld);
}

/*  
    Return the GameComponent one grid next to (x, y) in the specified direction.
    Return NULL if that grid is out of map
 */ 
GameComponent* Game::getGameComponentInDirection (int x, 
                                            int y, Direction direction) 
{
    switch (direction){
        case NORTH:
            if (y <= 0) return NULL;
            return gameGrids[x][y-1];
        case EAST:
            if (x >= MAP_WIDTH - 1) return NULL;
            return gameGrids[x+1][y];
        case SOUTH:
            if (y >= MAP_HEIGHT - 1) return NULL;
            return gameGrids[x][y+1];
        case WEST:
            if (x <= 0) return NULL;
            return gameGrids[x-1][y];
        default:
            return NULL;
            break;
    }
}

/**
 * GameUpdate struct
 * struct GameUpdate {
 *     UPDATE_TYPE update;
 *     int id;
 *     int gridDeltaX;
 *     int gridDeltaY;
 *     float floatDeltaX;
 *     float floatDeltaY;
 * };
 */

void Game::addUpdate(UPDATE_TYPE updateType, int id, int gridDeltaX, int gridDeltaY, float floatDeltaX, float floatDeltaY) {
    GameUpdate update;
    update.updateType = updateType;
    update.id = id;
    update.gridDeltaX = gridDeltaX;
    update.gridDeltaY = gridDeltaY;
    update.floatDeltaX = floatDeltaX;
    update.floatDeltaY = floatDeltaY;

    updates.push_back(update);
}
void Game::clearUpdates() {
    updates.clear();
}
