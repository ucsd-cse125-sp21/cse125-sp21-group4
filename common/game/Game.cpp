#include "Game.h"
#include "Obstacle.h"
#include "Monster.h"
#include "Fighter.h"
#include "Space.h"
#include "Rock.h"
#include "GamePlayer.h"
#include "HealingObjtv.h"

#define ROCK_SYMBOL 'R'
#define SPACE_SYMBOL ' '
#define MONSTER_SYMBOL 'M'
#define FIGHTER_SYMBOL 'F'
#define OBSTACLE_SYMBOL '+'
#define MONSTER_EVO_SYMBOL 'E'
#define FIGHTER_HEAL_SYMBOL 'H'
#define MONSTER_HEAL_SYMBOL 'J'


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
    players[0]->setID(0);

    position.x = P2_SPAWN_POSITION[0];
    position.y = P2_SPAWN_POSITION[1];
    players[1] = new Fighter(position);
    players[1]->setID(1);

    
    position.x = P3_SPAWN_POSITION[0];
    position.y = P3_SPAWN_POSITION[1];
    players[2] = new Fighter(position);
    players[2]->setID(2);


    position.x = P4_SPAWN_POSITION[0];
    position.y = P4_SPAWN_POSITION[1];
    position.width = MONSTER_WIDTH;
    position.height = MONSTER_HEIGHT;
    players[3] = new Monster(position);
    players[3]->setID(3);

}



/* return true if (x, y) is the boundary */
bool isBoundary (int widthIndex, int heightIndex) {
    if (widthIndex == 0) return true; // upper boundary
    if (heightIndex == 0) return true; // left boudary
    if (widthIndex == MAP_WIDTH / GRID_WIDTH - 1) return true; // bottom boundary
    if (heightIndex == MAP_HEIGHT / GRID_HEIGHT - 1) return true; // right boundary

    return false;
}

/* return the correct symbol for the object at cell. Return '' (blank char) for invalid symbols */
char getSymbol(GridComponent* cell) {
    switch(cell->getType()) {
        case OBSTACLE:
            return OBSTACLE_SYMBOL;

        case ROCK:
            return ROCK_SYMBOL;
        
        case SPACE:
            return SPACE_SYMBOL;

        case OBJECTIVE:
            if( ((Objective*) cell)->getObjective() == HEAL) {
                if ( ((Objective*) cell)->getRestriction() == R_FIGHTER)
                    return FIGHTER_HEAL_SYMBOL;
                else
                    return MONSTER_HEAL_SYMBOL;
            }

        default:
            return '\0';
    }
    // if (cell->getType() == OBSTACLE) return OBSTACLE_SYMBOL;
    // else if (cell->getType() == ROCK) return ROCK_SYMBOL;
    // else if (cell->isSpace()) return SPACE_SYMBOL;
    
    // return '\0';
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

    // testing purposes
    int test_rock_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_rock_height = (MAP_HEIGHT / GRID_HEIGHT) / 2;

    int test_fheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_fheal_height = ((MAP_WIDTH / GRID_WIDTH) / 2) - 1;

    int test_mheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_mheal_height = ((MAP_WIDTH / GRID_WIDTH) / 2) - 2;

    for (int i = 0; i < MAP_HEIGHT / GRID_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH / GRID_WIDTH; j++) {
            GridPosition position;
            position.x = j;
            position.y = i;
            // put obstacles on the boundary
            if (isBoundary(j, i))
                gameGrids[i][j] = new Obstacle(position);
            else {
                
                // testing purposes
                if(i == test_rock_width && j == test_rock_height) 
                    gameGrids[i][j] = new Rock(position);
                else if (i == test_fheal_width && j == test_fheal_height)
                    gameGrids[i][j] = new Heal(position, R_FIGHTER);
                else if (i == test_mheal_width && j == test_mheal_height)
                    gameGrids[i][j] = new Heal(position, R_MONSTER);
                else 
                    gameGrids[i][j] = new Space(position);

            }
        }
    }
}

void Game::cleanGameGrids() {
    // delete all the GameComponents related to the game grids
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
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

/*
    Return current GameState including players' positions and hp
*/
GameState Game::getGameState() {
    GameState gameState = GameState();
    for (int i = 0; i < PLAYER_NUM; i++) {
        gameState.playersPosition[i] = players[i]->getPosition();
        gameState.playersHp[i] = players[i]->getHp();
    }
    return gameState;
}

/*
    Return true if s1 and s2 contain the same content
*/
bool Game::sameGameState(GameState s1, GameState s2) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (!GamePlayer::samePosition(s1.playersPosition[i], s2.playersPosition[i]))
            return false;
        if (s1.playersHp[i] != s2.playersHp[i])
            return false; 
    }
    return true;
}

void Game::printGameState (GameState gameState) {
    cout << "------ Current Game State --------" << "\n";
    for (int i = 0; i < PLAYER_NUM; i++) {
        cout << "Player " << (i+1) << ": ";
        cout << "(" << gameState.playersPosition[i].x << ", ";
        cout << gameState.playersPosition[i].y << "), ";
        cout << "hp: " << gameState.playersHp[i] << "\n";
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
            char symbol = getSymbol(gameGrids[i][j]);

            if (symbol == '\0') cout << "ERROR: Encountered an unknown grid type!" << endl;
            else cout << getSymbol(gameGrids[i][j]) << " ";
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

/* =========================================================================
   Server side methods used to queue updates for the client to process 
========================================================================= */
void Game::addUpdate(GameUpdate update) {
    updates.push_back(update);
}
void Game::clearUpdates() {
    updates.clear();
}

/* =========================================================================
   Client side methods used to process updates.
========================================================================= */
void Game::handleUpdates(std::vector<GameUpdate> updates) {
    int numOfUpdates = updates.size();
    if(numOfUpdates <= 0) {
        return;
    }

    for(int i = 0; i < numOfUpdates; i++) {
        handleUpdate(updates[i]);
    }
}

void Game::handleUpdate(GameUpdate update) {
    // Later: we add an Animator/Graphics obj that does these animations.
    // As for the server side, we can just have an animator obj that does nothing.
    switch(update.updateType) {
        case PLAYER_DAMAGE_TAKEN:
            players[update.id]->hpDecrement(update.damageTaken);
            // Animate or update on graphics here.
            break;
        case PLAYER_MOVE:
            // Note: I do not do checks here because the server handles checks.
            PlayerPosition newPosition = players[update.id]->getPosition();
            newPosition.x += update.floatDeltaX;
            newPosition.y += update.floatDeltaY;
            players[update.id]->setPosition(newPosition);
            // Animate or update on graphics here.
            break;
        case PROJECTILE_MOVE:
            // Projectile can be identified with update.id.
            // Animate or update on graphics here.
            break;
        case OBJECTIVE_BEING_TAKEN:
            // Obj identified by update.gridPos.
            // Animate or update on graphics here.
            break;
        case OBJECTIVE_TAKEN:
            // Make the obj disappear? 
            // Obj identified by update.gridPos.
            // Animate or update on graphics here.
            break;
        default:
            printf("Not Handled Update Type: %d", update.updateType);
            break;
    }
}