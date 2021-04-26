#include "Game.h"
#include "Obstacle.h"
#include "Monster.h"
#include "Fighter.h"
#include "Rogue.h"
#include "Space.h"
#include "Rock.h"
#include "Evolve.h"
#include "GamePlayer.h"
#include "Armor.h"
#include "Healing.h"
#include "Beacon.h"

#define ROCK_SYMBOL 'R'
#define SPACE_SYMBOL ' '
#define BEACON_SYMBOL 'B'
#define MONSTER_SYMBOL 'M'
#define FIGHTER_SYMBOL 'F'
#define OBSTACLE_SYMBOL '+'
#define MONSTER_EVO_SYMBOL 'E'
#define FIGHTER_HEAL_SYMBOL 'H'
#define MONSTER_HEAL_SYMBOL 'J'
#define FIGHTER_ARMOR_SYMBOL 'A'

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
    players[1] = new Rogue(position);
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
    Objective* obj = (Objective*) cell;

    switch(cell->getType()) {
        case OBSTACLE:
            return OBSTACLE_SYMBOL;

        case ROCK:
            return ROCK_SYMBOL;
        
        case SPACE:
            return SPACE_SYMBOL;

        case OBJECTIVE:
            if(obj->getObjective() == HEAL) {
                if (obj->getRestriction() == R_FIGHTER)
                    return FIGHTER_HEAL_SYMBOL;
                else
                    return MONSTER_HEAL_SYMBOL;
            }
            else if(obj->getObjective() == EVO)
                return MONSTER_EVO_SYMBOL;
            else if(obj->getObjective() == ARMOR)
                return FIGHTER_ARMOR_SYMBOL;
            else if(obj->getObjective() == BEAC)
                return BEACON_SYMBOL;

        default:
            return '\0';
    }
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
    int test_fheal_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 1;

    int test_mheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_mheal_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 2;
    
    int test_mevo_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_mevo_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 3;

    int test_farm_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_farm_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 4;

    int test_beac_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_beac_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 5;

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
                if(j == test_rock_width && i == test_rock_height) 
                    gameGrids[i][j] = new Rock(position);
                else if (j == test_fheal_width && i == test_fheal_height)
                    gameGrids[i][j] = new Heal(position, R_FIGHTER);
                else if (j == test_mheal_width && i == test_mheal_height)
                    gameGrids[i][j] = new Heal(position, R_MONSTER);
                else if (j == test_mevo_width && i == test_mevo_height)
                    gameGrids[i][j] = new Evolve(position);
                else if (j == test_farm_width && i == test_farm_height)
                    gameGrids[i][j] = new Armor(position);
                else if (j == test_beac_width && i == test_beac_height)
                    gameGrids[i][j] = new Beacon(position);
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


bool Game::handleInputs(CLIENT_INPUT playersInputs[PLAYER_NUM]) {
    bool flag = false; // flag is true if there is effective movement in this round
                       // this is used to test on console, this can be removed later
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (playersInputs[i] != NO_MOVE) flag = true;
        players[i]->handleUserInput(this, playersInputs[i]);
    }
    return flag;
}

void projectileMove(Projectile* p) {
    switch (p->direction) {
        case SOUTH:
            p->currentPosition.y += p->speed;
            break;
        case NORTH:
            p->currentPosition.y -= p->speed;
            break;
        case WEST:
            p->currentPosition.x -= p->speed;
            break;
        case EAST:
            p->currentPosition.x += p->speed;
            break;
        default:
            break;
    }
}


/*
    Return true if this projectile is end, a projectile is
    considered as end in the following situations
    1. flying distance exceeds maxDistance
    2. outside of the map
    3. hit the obstacle

    Note: isEnd will not check any collision
*/
bool projectileIsEnd(Projectile* p, Game* game) {
    // 1. flying distance exceeds maxDistance
    if (p->direction == SOUTH || p->direction == NORTH) {
        if (abs(p->currentPosition.y - p->origin.y) > p->maxDistance) return true;
    } else {
        if (abs(p->currentPosition.x - p->origin.x) > p->maxDistance) return true; 
    }

    float x = p->currentPosition.x;
    float y = p->currentPosition.y;

    // 2. outside of the map
    if (x > MAP_WIDTH || x < 0) return true;
    if (y > MAP_HEIGHT || y < 0) return true;

    // hit the obstacle
    if (game->gameGrids[int(x/GRID_WIDTH)][int(y/GRID_HEIGHT)]->isObstacle())
        return true;
    
    return false;
}

/*
    Return true if the current position overlaps with enemy(s) positions,
    and decrease HP for the enemy(s) correspondingly.

    Note:
    1. Projectile will continue even if it hits teamates
    2. Projectile can cause damages for all the enemies being hit (could be more than 1)
*/
bool projectileIsCollidingEnemy (Projectile* p, Game* game) {
    float x = p->currentPosition.x;
    float y = p->currentPosition.y;

    bool hit = false;

    for (int i = 0; i < PLAYER_NUM; i++) {
        // can only attack enemy
        if (game->players[i]->getType() == MONSTER && p->ownerType == MONSTER) continue;
        if (game->players[i]->getType() != MONSTER && p->ownerType != MONSTER) continue;

        GamePlayer* otherPlayer = game->players[i];
        float p2ULX = otherPlayer->getUpperLeftCoordinateX(otherPlayer->getPosition(), true);
        float p2ULY = otherPlayer->getUpperLeftCoordinateY(otherPlayer->getPosition(), true);
        float p2BRX = otherPlayer->getBottomRightCoordinateX(otherPlayer->getPosition(), true);
        float p2BRY = otherPlayer->getBottomRightCoordinateY(otherPlayer->getPosition(), true);

        if (x >= p2ULX && x <= p2BRX && y >= p2ULY && y <= p2BRY) {
            hit = true;
            otherPlayer->hpDecrement(p->damage);

            // queue this update to be send to other players
            GameUpdate gameUpdate;
            gameUpdate.updateType = PLAYER_DAMAGE_TAKEN;
            gameUpdate.id = i;
            gameUpdate.damageTaken = p->damage;
            game->addUpdate(gameUpdate);
        }

    }

    return hit;
}


/*
    Update the status of each projectiles
    If the projectile has reached the maxDistance, or hit an obstacle, remove.
    If the projectile has hit one of the enemy(s), remove and damage the 
    enemy correspondingly.
*/
void Game::updateProjectiles () {
    std::vector<Projectile*> newProjectiles;
    for (auto iter = projectiles.begin(); iter != projectiles.end(); iter++) {
        // move
        projectileMove(*iter);

        // remove if exceeds max distance or hit boundary
        if (projectileIsEnd(*iter, this)) {
            delete (*iter); // free the space
            continue;
        }

        // check for collision with players
        if ((projectileIsCollidingEnemy(*iter, this))) {
            // if this has hit an enemy, remove the projectile
            delete (*iter); // free the space
            continue;
        }
        newProjectiles.push_back(*iter);
    }
    projectiles = newProjectiles;
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

    printStats();
}

/* =========================================================================
   Server side methods used to queue updates for the client to process 
========================================================================= */
void Game::addUpdate(GameUpdate update) {
    // printf("Server push update");
    updates.push_back(update);
}
void Game::clearUpdates() {
    updates.clear();
}

/* =========================================================================
   Client side methods used to process updates.
   Note: Graphics Client currently doesn't have a Game instance. Not sure if 
         we will be separating Game vs ClientGame. But I will include these 
         methods in the case we use Game.cpp in server AND client.
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

// Note: May or may not call graphics/animator object. Depends if Game instance is
// added to the graphics client. If we add Game instance, I would recommend creating
// an Animator object that is responsible for calling methods on Window to update graphics.
void Game::handleUpdate(GameUpdate update) {
    switch(update.updateType) {
        case PLAYER_DAMAGE_TAKEN:
            players[update.id]->hpDecrement(update.damageTaken);
            break;
        case PLAYER_MOVE:
        // Need curly braces because I am declaring new variables inside the case statement
        {
            // Note: I do not do checks here because the server handles checks.
            PlayerPosition newPosition = players[update.id]->getPosition();
            newPosition.x += update.floatDeltaX;
            newPosition.y += update.floatDeltaY;
            players[update.id]->setPosition(newPosition);
            break;
        }
        case PROJECTILE_MOVE:
            // Projectile can be identified with update.id.
            break;
        case OBJECTIVE_BEING_TAKEN:
            // Obj identified by update.gridPos.
            break;
        case OBJECTIVE_TAKEN:
            // Make the obj disappear? 
            // Obj identified by update.gridPos.
            break;
        default:
            printf("Not Handled Update Type: %d", update.updateType);
            break;
    }
}
// testing  purposes
void Game::printStats() {

    int test_rock_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_rock_height = (MAP_HEIGHT / GRID_HEIGHT) / 2;

    int test_fheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_fheal_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 1;

    int test_mheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_mheal_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 2;
    
    int test_mevo_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_mevo_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 3;

    int test_farm_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_farm_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 4;

    int test_beac_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    int test_beac_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 5;

    cout << "Fighter Heal amount - " << ((Heal*) gameGrids[test_fheal_height][test_fheal_width])->getHealAmount() << endl;
    cout << "Monster Heal amount - " << ((Heal*) gameGrids[test_mheal_height][test_mheal_width])->getHealAmount() << endl;
    cout << "Monster Evo amount - " << ((Evolve*) gameGrids[test_mevo_height][test_mevo_width])->getEvoAmount() << endl;
    cout << "Fighter Armor amount - " << ((Armor*) gameGrids[test_farm_height][test_farm_width])->getArmorAmount() << endl;
    cout << "Beacon Frequency (units unknown) - " << ((Beacon*) gameGrids[test_beac_height][test_beac_width])->getFrequency() << endl;   
}