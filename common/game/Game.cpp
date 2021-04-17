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
    initPlayers();
}


/*
    Initialize players in the game
    First three players will be fighters and the fourth will be monster
*/
void Game::initPlayers () {
    PlayerPosition position = PlayerPosition();
    position.x = P1_SPAWN_POSITION[0];
    position.y = P1_SPAWN_POSITION[1];
    position.width = FIGHTER_WIDTH;
    position.height = FIGHTER_HEIGHT;
    players[0] = new Fighter(position);

    position.x = P2_SPAWN_POSITION[0];
    position.y = P2_SPAWN_POSITION[1];
    players[1] = new Fighter(position);
    
    position.x = P3_SPAWN_POSITION[0];
    position.y = P3_SPAWN_POSITION[1];
    players[2] = new Fighter(position);

    position.x = P4_SPAWN_POSITION[0];
    position.y = P4_SPAWN_POSITION[1];
    position.width = MONSTER_WIDTH;
    position.height = MONSTER_HEIGHT;
    players[3] = new Monster(position);
}



/* return true if (x, y) is the boundary */
bool isBoundary (int widthIndex, int heightIndex) {
    if (widthIndex == 0) return true; // upper boundary
    if (heightIndex == 0) return true; // left boudary
    if (widthIndex == MAP_WIDTH / GRID_WIDTH - 1) return true; // bottom boundary
    if (heightIndex == MAP_HEIGHT / GRID_HEIGHT - 1) return true; // right boundary

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
    for (int i = 0; i < MAP_WIDTH / GRID_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT / GRID_HEIGHT; j++) {
            GridPosition position;
            position.x = i;
            position.y = j;
            // put obstacles on the boundary
            if (isBoundary(i, j))
                gameGrids[i][j] = new Obstacle(position);
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
}

void Game::cleanPlayers() {
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (!players[i]) delete players[i];
        players[i] = NULL;
    }
}

Game::~Game() {
    cleanGameGrids();
    cleanPlayers();
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

void Game::printGameGrids () {
    for (int i = 0; i < MAP_HEIGHT / GRID_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH / GRID_WIDTH; j++) {
            if (gameGrids[j][i]->isObstacle()) cout << OBSTACLE_SYMBOL;
            else if (gameGrids[j][i]->isSpace()) cout << SPACE_SYMBOL;
            else cout << "ERROR: Encounter unknown grid type!" << endl;
        }
        cout << "\n";
    }
}

void Game::printPlayers () {
    for (int i = 0; i < PLAYER_NUM; i++) {
        cout << "Player " << (i+1) << ": ";
        cout << "(" << players[i]->getPosition().x << ", ";
        cout << players[i]->getPosition().y << "), ";
        cout << "hp: " << players[i]->getHp() << "\n";
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
