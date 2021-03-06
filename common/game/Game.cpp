#include "Game.h"
#include "Obstacle.h"
#include "Monster.h"
#include "Fighter.h"
#include "Rogue.h"
#include "Mage.h"
#include "Cleric.h"
#include "Space.h"
#include "Rock.h"
#include "Evolve.h"
#include "GamePlayer.h"
#include "Armor.h"
#include "Healing.h"
#include "Beacon.h"

// Map symbols
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

int Objective::globalObjectiveIDCounter = 0; // used to keep count on every objective

Game::Game() {
    initGameGrids();
    // initPlayers(); Will init players when the game starts and we know the jobs
    initSelectScreenStructures();

    lastSafeRegionShrinkTime = std::chrono::steady_clock::now();
    lastSafeRegionAttackTime = std::chrono::steady_clock::now();

    // initialize player's liveness list
    for (int i = 0; i < PLAYER_NUM; i++) {
        prevPlayerStatus[i] = true;
    }
}

// initializes the map structure for easier selecting of jobs
void Game::initSelectScreenStructures() {
    renderCount = 0;
    availableJobs = {CLERIC, FIGHTER, MAGE, ROGUE};
    started = false; // game will not start for another 30 seconds
    
    idToJobType[0] = UNKNOWN; // UNKNOWN == has not chosen
    idToJobType[1] = UNKNOWN; 
    idToJobType[2] = UNKNOWN;
    idToJobType[3] = MONSTER; // Player 3 is always MONSTER for now
}

/*
    Initialize players in the game
    First three players will be fighters and the fourth will be monster
*/
void Game::initPlayers () {

    // For each player, get their positions and set their classes
    for (int i = 0; i < MAX_PLAYERS; i++) {
        
        PlayerPosition position = PlayerPosition();
        position.x = SPAWN_POSITIONS[i][0];
        position.y = SPAWN_POSITIONS[i][1];
        switch(idToJobType[i]) {
            case CLERIC:
                position.width = CLERIC_WIDTH;
                position.height = CLERIC_HEIGHT;
                players[i] = new Cleric(position);
                players[i]->setID(i);
                break;
            case FIGHTER:
                position.width = FIGHTER_WIDTH;
                position.height = FIGHTER_HEIGHT;
                players[i] = new Fighter(position);
                players[i]->setID(i);
                break;
            case MAGE:
                position.width = MAGE_WIDTH;
                position.height = MAGE_HEIGHT;
                players[i] = new Mage(position);
                players[i]->setID(i);
                break;
            case ROGUE:
                position.width = ROGUE_WIDTH;
                position.height = ROGUE_HEIGHT;
                players[i] = new Rogue(position);
                players[i]->setID(i);
                break;
            case MONSTER:
                position.width = MONSTER_WIDTH;
                position.height = MONSTER_HEIGHT;
                players[i] = new Monster(position);
                players[i]->setID(i);
                break;
            case UNKNOWN: {
                PlayerType availableJob = *availableJobs.begin();
                idToJobType[i] = availableJob;
                availableJobs.erase(availableJob);
                i--; // decrement to restart the process for this player

                // We want to send ROLE_CLAIMED update for this player because it was not sent before.
                GameUpdate gameUpdate;
                gameUpdate.updateType = ROLE_CLAIMED;
                gameUpdate.id = i + 1;
                gameUpdate.roleClaimed = availableJob;
                addUpdate(gameUpdate);
                break;
            }
            default:
                break;
        }
    }

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
                if (obj->getRestriction() == R_HUNTER)
                    return FIGHTER_HEAL_SYMBOL;
                else
                    return MONSTER_HEAL_SYMBOL;
            }
            else if(obj->getObjective() == EVO)
                return MONSTER_EVO_SYMBOL;
            else if(obj->getObjective() == ARMOR)
                return FIGHTER_ARMOR_SYMBOL;
            else if(obj->getObjective() == BEACON)
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
//<<<<<<< 
    // int test_rock_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_rock_height = (MAP_HEIGHT / GRID_HEIGHT) / 2;

    // int test_fheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_fheal_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 1;

    // int test_mheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_mheal_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 2;
    
    // int test_mevo_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_mevo_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 3;

    // int test_farm_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_farm_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 4;

    // int test_beac_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_beac_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 5;

    // for (int i = 0; i < MAP_HEIGHT / GRID_HEIGHT; i++) {
    //     for (int j = 0; j < MAP_WIDTH / GRID_WIDTH; j++) {
    //         GridPosition position;
    //         position.x = j;
    //         position.y = i;
    //         // put obstacles on the boundary
    //         if (isBoundary(j, i))
    //             gameGrids[i][j] = new Obstacle(position);
    //         else {
                
    //             // testing purposes
    //             if(j == test_rock_width && i == test_rock_height) 
    //                 gameGrids[i][j] = new Rock(position);
    //             else if (j == test_fheal_width && i == test_fheal_height)
    //                 gameGrids[i][j] = new Heal(position, R_FIGHTER);
    //             else if (j == test_mheal_width && i == test_mheal_height)
    //                 gameGrids[i][j] = new Heal(position, R_MONSTER);
    //             else if (j == test_mevo_width && i == test_mevo_height)
    //                 gameGrids[i][j] = new Evolve(position);
    //             else if (j == test_farm_width && i == test_farm_height)
    //                 gameGrids[i][j] = new Armor(position);
    //             else if (j == test_beac_width && i == test_beac_height)
    //                 gameGrids[i][j] = new Beacon(position);
    //             else 
    //                 gameGrids[i][j] = new Space(position);

    //         }
    //     }
    // }
    ifstream map_file("../assets/layout/map_server.csv");
    string line;
    string id;

    int i = 0;
    int j = 0;

    while(getline(map_file, line)) {
        stringstream ss(line);
        
        while(getline(ss, id, ',')) {
            // printf("%s\n", id);
            GridPosition position;
            position.x = j;
            position.y = i;
            int objID = std::stoi(id);

            switch(objID) {
                case SPACE_ID: {
                    gameGrids[j][i] = new Space(position);
                    break;
                }
                case OBST_ID: {
                    gameGrids[j][i] = new Obstacle(position); 
                    break;
                }
                case HUNTER_HP_ID: {
                    gameGrids[j][i] = new Heal(position, R_HUNTER);
                    Objective* obj = (Objective *)gameGrids[j][i];
                    objectives.push_back(obj);
                    
                    // Send update to client that objective have spawned
                    GameUpdate objSpawnUpdate;
                    objSpawnUpdate.updateType = SPAWN_OBJECTIVE;
                    objSpawnUpdate.objectiveSpawnType = obj->getObjective();
                    objSpawnUpdate.objRestrictionType = obj->getRestriction();
                    objSpawnUpdate.objectiveID = obj->getObjectiveID();
                    objSpawnUpdate.gridPos = position;
                    this->addUpdate(objSpawnUpdate);
                    break;
                }
                case HUNTER_ARMOR_ID: {
                    gameGrids[j][i] = new Armor(position);
                    Objective* obj = (Objective *)gameGrids[j][i];
                    objectives.push_back(obj);
                    
                    // Send update to client that objective have spawned
                    GameUpdate objSpawnUpdate;
                    objSpawnUpdate.updateType = SPAWN_OBJECTIVE;
                    objSpawnUpdate.objectiveSpawnType = obj->getObjective();
                    objSpawnUpdate.objRestrictionType = obj->getRestriction();
                    objSpawnUpdate.objectiveID = obj->getObjectiveID();
                    objSpawnUpdate.gridPos = position;
                    this->addUpdate(objSpawnUpdate);
                    break;
                }
                case MONSTER_HP_ID: {
                    gameGrids[j][i] = new Heal(position, R_MONSTER);
                    Objective* obj = (Objective *)gameGrids[j][i];
                    objectives.push_back(obj);
                    
                    // Send update to client that objective have spawned
                    GameUpdate objSpawnUpdate;
                    objSpawnUpdate.updateType = SPAWN_OBJECTIVE;
                    objSpawnUpdate.objectiveSpawnType = obj->getObjective();
                    objSpawnUpdate.objRestrictionType = obj->getRestriction();
                    objSpawnUpdate.objectiveID = obj->getObjectiveID();
                    objSpawnUpdate.gridPos = position;
                    this->addUpdate(objSpawnUpdate);
                    break;
                }
                case MONSTER_EVOLVE_ID: {
                    gameGrids[j][i] = new Evolve(position);
                    Objective* obj = (Objective *)gameGrids[j][i];
                    objectives.push_back(obj);
                    
                    // Send update to client that objective have spawned
                    GameUpdate objSpawnUpdate;
                    objSpawnUpdate.updateType = SPAWN_OBJECTIVE;
                    objSpawnUpdate.objectiveSpawnType = obj->getObjective();
                    objSpawnUpdate.objRestrictionType = obj->getRestriction();
                    objSpawnUpdate.objectiveID = obj->getObjectiveID();
                    objSpawnUpdate.gridPos = position;
                    this->addUpdate(objSpawnUpdate);
                    break;
                }
                case BEAC_ID: {
                    gameGrids[j][i] = new Beacon(position);
                    Objective* obj = (Objective *)gameGrids[j][i];
                    beacon = (Beacon*) gameGrids[j][i];
                    
                    // Send update to client that objective have spawned
                    GameUpdate objSpawnUpdate;
                    objSpawnUpdate.updateType = SPAWN_OBJECTIVE;
                    objSpawnUpdate.objectiveSpawnType = obj->getObjective();
                    objSpawnUpdate.objRestrictionType = obj->getRestriction();
                    objSpawnUpdate.objectiveID = obj->getObjectiveID();
                    objSpawnUpdate.gridPos = position;
                    this->addUpdate(objSpawnUpdate);
                    break;
                }
                default: {
                    printf("Invalid integer in map_server.csv, putting Space Object for now\n");
                    gameGrids[j][i] = new Space(position);
                    break;
                }
            }

            // printf("%d, %d, %d\n", objID, i, j);

            j++;
        }
        j = 0;
        i++;
    }
}

void Game::startSelectTimer() {
    // Give players SELECT_SCREEN_TIME seconds to select jobs
    GameEvent* gameStartEvent = new GameEvent();
    gameStartEvent->type = GAME_START;
    gameStartEvent->time = std::chrono::steady_clock::now() + 
                    std::chrono::seconds(SELECT_SCREEN_TIME);
    this->events.push_back(gameStartEvent);
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


bool Game::handleInputs(GAME_INPUT playersInputs[PLAYER_NUM]) {
    bool flag = false; // flag is true if there is effective movement in this round
                       // this is used to test on console, this can be removed later
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (playersInputs[i].input != NO_MOVE) flag = true;

        // The selecting screen should not rely on players because those objects have not been created
        switch(playersInputs[i].input) {
            case CLAIM_CLERIC:
            case CLAIM_FIGHTER:
            case CLAIM_MAGE:
            case CLAIM_ROGUE:
                // If game started, we shouldn't be handling this.
                if(!started) {
                    handleUserClaim(playersInputs[i].input, i);
                }
                break;
            case DONE_RENDERING: {
                bool ready = true;
                doneRenderedClients[i] = true;
                printf("Player %d has finished rendering.\n", i);
                for(int i = 0; i < PLAYER_NUM; i++) {
                    if(!doneRenderedClients[i]) {
                        ready = false;
                    }
                }
                if(ready) {
                    // All players have joined
                    GameUpdate allJoined;
                    allJoined.updateType = ALL_PLAYERS_JOINED;
                    allJoined.selectTimerStartTime = std::chrono::steady_clock::now();
                    this->addUpdate(allJoined);

                    // All players have connected by now, so start the select timer
                    startSelectTimer();
                }
                break;
            }
            default:
                if(started) {
                    players[i]->handleUserInput(this, playersInputs[i]);
                }
                break;
        }
    }
    return flag;
}


void Game::handleUserClaim (CLIENT_INPUT claimType, int playerID) {

    // If player has not locked into a role yet.
    if(idToJobType[playerID] == UNKNOWN) {
        switch(claimType) {
            case CLAIM_CLERIC:
                if(availableJobs.find(CLERIC) != availableJobs.end()) {
                    idToJobType[playerID] = CLERIC;
                    availableJobs.erase(CLERIC);

                    // send update to the rest of the clients
                    GameUpdate gameUpdate;
                    gameUpdate.updateType = ROLE_CLAIMED;
                    gameUpdate.id = playerID;
                    gameUpdate.roleClaimed = CLERIC;
                    addUpdate(gameUpdate);
                }
                break;
            case CLAIM_ROGUE:
                if(availableJobs.find(ROGUE) != availableJobs.end()) {
                    idToJobType[playerID] = ROGUE;
                    availableJobs.erase(ROGUE);

                    // send update to the rest of the clients
                    GameUpdate gameUpdate;
                    gameUpdate.updateType = ROLE_CLAIMED;
                    gameUpdate.id = playerID;
                    gameUpdate.roleClaimed = ROGUE;
                    addUpdate(gameUpdate);
                }
                break;
            case CLAIM_FIGHTER:
                if(availableJobs.find(FIGHTER) != availableJobs.end()) {
                    idToJobType[playerID] = FIGHTER;
                    availableJobs.erase(FIGHTER);

                    // send update to the rest of the clients
                    GameUpdate gameUpdate;
                    gameUpdate.updateType = ROLE_CLAIMED;
                    gameUpdate.id = playerID;
                    gameUpdate.roleClaimed = FIGHTER;
                    addUpdate(gameUpdate);
                }
                break;
            case CLAIM_MAGE:
                if(availableJobs.find(MAGE) != availableJobs.end()) {
                    idToJobType[playerID] = MAGE;
                    availableJobs.erase(MAGE);

                    // send update to the rest of the clients
                    GameUpdate gameUpdate;
                    gameUpdate.updateType = ROLE_CLAIMED;
                    gameUpdate.id = playerID;
                    gameUpdate.roleClaimed = MAGE;
                    addUpdate(gameUpdate);
                }
                break;
            default:
                printf("Error: handleUserClaim got invalid CLIENT_INPUT.\n");
                break;
        }
    }
}

void projectileMove(Projectile* p) {
    p->currentPosition.x += p->deltaX;
    p->currentPosition.y += p->deltaY;
}


/*
    Return true if this projectile is end, a projectile is
    considered as end in the following situations
    1. flying distance exceeds maxDistance
    2. outside of the map
    3. hit the obstacle

    Note: checkEnd will not check any collision
*/
bool projectileIsEnd(Projectile* p, Game* game) {
    // 1. flying distance exceeds maxDistance
    if (pow(p->currentPosition.x - p->origin.x, 2) >= pow(p->maxDistance, 2))
        return true;
    if (pow(p->currentPosition.y - p->origin.y, 2) >= pow(p->maxDistance, 2))
        return true;

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
        // skip dead players
        GamePlayer* otherPlayer = game->players[i];
        if (otherPlayer->isDead()) continue;

        // can only attack enemy
        if (!game->players[p->ownerID]->canAttack(otherPlayer)) continue;

        float p2ULX = otherPlayer->getUpperLeftCoordinateX(otherPlayer->getPosition(), true);
        float p2ULY = otherPlayer->getUpperLeftCoordinateY(otherPlayer->getPosition(), true);
        float p2BRX = otherPlayer->getBottomRightCoordinateX(otherPlayer->getPosition(), true);
        float p2BRY = otherPlayer->getBottomRightCoordinateY(otherPlayer->getPosition(), true);

        if (x >= p2ULX && x <= p2BRX && y >= p2ULY && y <= p2BRY) {
            hit = true;

            // projectile will cause effect
            if (p->type == MAGE_FIREBALL) {
                otherPlayer->slowDown(FIREBALL_SPEED_DEC);
                // restrict player's max speed
                switch (otherPlayer->getType())
                {
                    case MONSTER:
                        otherPlayer->setMaxSpeed(MONSTER_MAX_SPEED / 2);
                        ((Monster*) otherPlayer)->setFire(true);
                        break;
                    case FIGHTER:
                        otherPlayer->setMaxSpeed(FIGHTER_MAX_SPEED / 2);
                        break;
                    case MAGE:
                        otherPlayer->setMaxSpeed(MAGE_MAX_SPEED / 2);
                        break;
                    case CLERIC:
                        otherPlayer->setMaxSpeed(CLERIC_MAX_SPEED / 2);
                        break;
                    case ROGUE:
                        otherPlayer->setMaxSpeed(ROGUE_MAX_SPEED / 2);
                        break;                
                    default:
                        break;
                }
                // create an event to add the speed back later
                GameEvent* event = new GameEvent();
                event->type = SPEED_CHANGE;
                event->ownerID = p->ownerID;
                event->targetID = i;
                event->amount = FIREBALL_SPEED_DEC;
                event->time = std::chrono::steady_clock::now() + 
                                std::chrono::milliseconds(FIREBALL_EFFECT_TIME);
                game->events.push_back(event);

                event = new GameEvent();
                event->type = FIREBALL_END;
                event->ownerID = p->ownerID;
                event->targetID = i;
                event->time = std::chrono::steady_clock::now() + 
                                std::chrono::milliseconds(FIREBALL_EFFECT_TIME);
                game->events.push_back(event);

                // send speed dec effect to client
                GameUpdate gameUpdate;
                gameUpdate.updateType = HIT_BY_MAGE_FIREBALL;
                gameUpdate.id = i;
                game->addUpdate(gameUpdate);
            }
            // projectile will only cause damage 
            else {
                otherPlayer->hpDecrement(p->damage, false);

                // if fighter's shield is on, it cannot be damaged by other players
                if (otherPlayer->getType() == FIGHTER && ((Fighter*) otherPlayer)->getShieldOn())
                    continue;
                // queue this update to be send to other players
                GameUpdate gameUpdate;
                gameUpdate.updateType = PLAYER_DAMAGE_TAKEN;
                gameUpdate.id = i;
                gameUpdate.damageTaken = p->damage;
                game->addUpdate(gameUpdate);
            }

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
    auto iter = projectiles.begin();
    while (iter != projectiles.end()) {
        // move
        projectileMove(iter->second);

        // remove if exceeds max distance or hit boundary or collide with other players
        if (projectileIsEnd(iter->second, this) || (projectileIsCollidingEnemy(iter->second, this))) {
            // if this has hit an enemy, remove the projectile

            // send projectile end state to each clients
            GameUpdate projectileUpdate;
            projectileUpdate.updateType = PROJECTILE_END;
            projectileUpdate.id = iter->first;
            addUpdate(projectileUpdate);

            delete (iter->second); // free the space
            iter = projectiles.erase(iter);

        } else {
            // send projectile state to each clients
            GameUpdate projectileUpdate;
            projectileUpdate.updateType = PROJECTILE_MOVE;
            projectileUpdate.id = iter->first;
            projectileUpdate.projectileType = iter->second->type;
            projectileUpdate.playerPos = {iter->second->currentPosition.x, iter->second->currentPosition.y};
            projectileUpdate.floatDeltaX = iter->second->deltaX;
            projectileUpdate.floatDeltaY = iter->second->deltaY;
            addUpdate(projectileUpdate);

            iter++;
        }
    }
}

/**
 *  updateBeacon() will check who is near the beacon and change the beacon's status based on
 *  whether or not it was captured or is being captured.
 */
void Game::updateBeacon() {

    // Captured Phase: just ping the enemy team
    if(beacon->isCaptured()) {

        // Ping if beacon can ping (tickCounter is full)
        if(beacon->canPing()) {
            
            // if it was captured by hunter
            if(beacon->getCaptureAmount() >= HUNTER_BEACON_CAPTURE_THRESHOLD) {

                // get position of monster and send it over as udpate
                for(int i = 0; i < PLAYER_NUM; i++) {
                    if(players[i]->getType() == MONSTER) {
                        Monster* monster = (Monster*) players[i];
                        GameUpdate monsterPosUpdate;
                        monsterPosUpdate.updateType = BEACON_PING_PLAYER;
                        monsterPosUpdate.playerPos = monster->getPosition();
                        monsterPosUpdate.id = monster->getID();
                        this->addUpdate(monsterPosUpdate);
                        break; // break, only one monster in the game.
                    }
                }

            // if it was captured by monster
            } else if (beacon->getCaptureAmount() <= MONSTER_BEACON_CAPTURE_THRESHOLD) {

                // Get positions of all hunters and send it over as updates
                for(int i = 0; i < PLAYER_NUM; i++) {
                    if(players[i]->getType() != MONSTER) {
                        GamePlayer* hunter = (GamePlayer*) players[i];
                        GameUpdate hunterPosUpdate;
                        hunterPosUpdate.updateType = BEACON_PING_PLAYER;
                        hunterPosUpdate.playerPos = hunter->getPosition();
                        hunterPosUpdate.id = hunter->getID();
                        this->addUpdate(hunterPosUpdate);
                    }
                }
            }

            // reset tick counter for the beacon
            beacon->resetTickCounter();

        // Otherwise, increment the tick counter until it can ping
        } else {
            beacon->incrementTickCounter();
        }


    // Capturing Phase: check if players are in the beacon range
    } else {
        float captureAmount = 0;
        bool playersInArea = false;
        for(int i = 0; i < PLAYER_NUM; i++) {
            GamePlayer* player = players[i];
            
            // Squared Distance
            GridPosition beaconPos = beacon->getPosition();
            PlayerPosition playerPos = player->getPosition();
            float squaredDistanceX =  pow(playerPos.x - beaconPos.x, 2);
            float squaredDistanceY = pow(playerPos.y - beaconPos.y, 2);

            // squared distance used instead of distance because less computation required.
            if (squaredDistanceX + squaredDistanceY <= pow(beacon->getInteractionRange(), 2)) {
                playersInArea = true;
                if(player->getType() == MONSTER) {
                    captureAmount -= beacon->MONSTER_CAPTURE_RATE;
                } else {
                    captureAmount += beacon->HUNTER_CAPTURE_RATE;
                }
            }


        }

        // If any players are in the area, captureAmount != 0
        if(captureAmount != 0 && playersInArea) {
            beacon->updateCaptureAmount(this, captureAmount);
            // printf("Capture Amount %f\n", captureAmount);

            // send capturing update to all players
            GameUpdate beaconCapturingUpdate;
            beaconCapturingUpdate.updateType = BEACON_BEING_TAKEN;
            beaconCapturingUpdate.beaconCaptureAmount = beacon->getCaptureAmount();
            this->addUpdate(beaconCapturingUpdate);

        // If no players are around, decay the beacon amount.
        } else if (beacon->getCaptureAmount() < DECAY_LOWER_THRESHOLD || beacon->getCaptureAmount() > DECAY_UPPER_THRESHOLD) {
            beacon->decayCaptureAmount();
            
            // send delaying update to all players
            GameUpdate beaconDecayingUpdate;
            beaconDecayingUpdate.updateType = BEACON_DECAYING;
            beaconDecayingUpdate.beaconCaptureAmount = beacon->getCaptureAmount();
            this->addUpdate(beaconDecayingUpdate);

        }
        
    }
}


/*
    This will be called on the server for each tick.
*/
void Game::updateSafeRegion () {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> duration = currentTime - lastSafeRegionShrinkTime;

    // if safe region hasn't appeared yet
    if (safeRegionRadius < 0 && std::chrono::duration_cast<std::chrono::seconds>(duration).count() > SAFE_REGION_FIRST_SPAWN_TIME) {
        shrinkSafeRegion();
        lastSafeRegionShrinkTime = currentTime;

    } else if (safeRegionRadius > 0 && std::chrono::duration_cast<std::chrono::seconds>(duration).count() > SAFE_REGION_DAMAGE_TIME) {
        shrinkSafeRegion();
        lastSafeRegionShrinkTime = currentTime;
    } else {
        return;
    }

    duration = currentTime - lastSafeRegionAttackTime;
    if (std::chrono::duration_cast<std::chrono::seconds>(duration).count() 
                                            > SAFE_REGION_DAMAGE_TIME) {
        attackPlayersOutsideSafeRegion();
        lastSafeRegionAttackTime = currentTime;
    }
}


/*
    Return 0 if the game is not end.
    Return 1 if hunters win.
    Return 2 if monster wins.
    Return 3 if there is a tie.
*/
bool Game::checkEnd() {
    int deadHunterNum = 0;
    int deadMonsterNum = 0;

    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i]->getType() == MONSTER && players[i]->isDead())
            deadMonsterNum += 1;
        else if (players[i]->getType() != MONSTER && players[i]->isDead())
            deadHunterNum += 1;
    }

    int endStatus = 0;
    // if monster died, return 1 if there is at least one hunter survived
    if (deadMonsterNum == 1)
        endStatus = deadHunterNum != PLAYER_NUM - 1 ? 1 : 3;
    // if all hunters died, return 2 if the monster is not dead
    else if (deadHunterNum == PLAYER_NUM - 1)
        endStatus = 2;
    // otherwise, game continues
    if (endStatus == 0) return false;

    GameUpdate gameEndUpdate;
    gameEndUpdate.updateType = GAME_END;
    gameEndUpdate.endStatus = endStatus;
    addUpdate(gameEndUpdate);
    return true;
}


void Game::updateDeath () {
    for (int i = 0; i < PLAYER_NUM; i++) {
        // prev tick is alive but this tick is dead
        if (prevPlayerStatus[i] && players[i]->isDead()) {
            // send dead status to client
            printf("send player dead signal  duolan \n");
            GameUpdate update;
            update.updateType = PLAYER_DEAD;
            update.id = i;
            addUpdate(update);
        // prev tick is dead but this tick is alive
        } else if (!prevPlayerStatus[i] && !players[i]->isDead()) {
            // send player being revived status to client
            // DONE IN GAMEPLAYER
            // GameUpdate update;
            // update.updateType = PLAYER_REVIVE;
            // update.id = i;
            // addUpdate(update);
        }

        // kinda trick here, prevPlayerStatus[i] is true if player is alive
        // but players[i]->isDead() is true if player is dead
        prevPlayerStatus[i] = !players[i]->isDead();
    }
}




void Game::shrinkSafeRegion () {
    if (safeRegionRadius < 0) safeRegionRadius = SAFE_REGION_START_RADIUS;
    else safeRegionRadius = max(safeRegionRadius-SAFE_REGION_RADIUS_DEC, (float) SAFE_REGION_MIN_RADIUS);

    GameUpdate gameUpdate;
    gameUpdate.updateType = SAFE_REGION_UPDATE;
    gameUpdate.playerPos.x = safeRegionX;
    gameUpdate.playerPos.y = safeRegionY;
    gameUpdate.beaconCaptureAmount = safeRegionRadius;
    addUpdate(gameUpdate);
}


void Game::attackPlayersOutsideSafeRegion () {
    if (safeRegionRadius < 0) return;

    for (int i = 0; i < PLAYER_NUM; i++) {
        float distanceSqr = pow(players[i]->getPosition().x - safeRegionX, 2) +
                       pow(players[i]->getPosition().y - safeRegionY, 2);

        // if a player is outside of safeRegion, it will be attacked by the system
        if (sqrt(distanceSqr) > safeRegionRadius) {
            float damage = 0;
            if (players[i]->getType() == MONSTER) damage = SAFE_REGION_MONSTER_DAMAGE;
            else damage = SAFE_REGION_HUNTER_DAMAGE;

            // If players are dead don't keep damaging them.
            if (players[i]->getHp() == 0) continue;

            players[i]->hpDecrement(damage, true);
            // queue this update to be send to other players
            GameUpdate gameUpdate;
            gameUpdate.updateType = PLAYER_DAMAGE_TAKEN;
            gameUpdate.id = i;
            gameUpdate.damageTaken = damage;
            addUpdate(gameUpdate);
        }
    }
}


/* Process a single event */
void Game::processEvent (GameEvent* event) {
    switch (event->type)
    {
        case HP_DEC: {
                players[event->targetID]->hpDecrement(event->amount, false);
                // queue this update to be send to other players
                GameUpdate gameUpdate;
                gameUpdate.updateType = PLAYER_DAMAGE_TAKEN;
                gameUpdate.id = event->targetID;
                gameUpdate.damageTaken = event->amount;
                addUpdate(gameUpdate);
            }
            break;
        case SPEED_CHANGE:
            players[event->targetID]->speedChange(event->amount);
            if(players[event->targetID]->getType() == MONSTER) {
                Monster* monsterPlayer = (Monster*)players[event->targetID];
                if(monsterPlayer->isOnFire()) {
                    monsterPlayer->setFire(false);
                }
            }
            break;
        case TAKE_FIGHTER_SHIELD_DOWN: {
                if (players[event->ownerID]->getType() != FIGHTER) break;
                ((Fighter*) players[event->ownerID])->setShieldOn(false);
                GameUpdate update;
                update.updateType = FIGHTER_SHIELD_DOWN;
                update.id = event->ownerID;                      
                addUpdate(update);
                break;
            }
        case FIREBALL_END: {
                GameUpdate gameUpdate;
                gameUpdate.updateType = RECOVER_FROM_MAGE_FIREBALL;
                gameUpdate.id = event->targetID;
                addUpdate(gameUpdate);

                // reset player's max speed
                switch (players[event->targetID]->getType())
                {
                    case MONSTER:
                        players[event->targetID]->setMaxSpeed(MONSTER_MAX_SPEED);
                        break;
                    case FIGHTER:
                        players[event->targetID]->setMaxSpeed(FIGHTER_MAX_SPEED);
                        break;
                    case MAGE:
                        players[event->targetID]->setMaxSpeed(MAGE_MAX_SPEED);
                        break;
                    case CLERIC:
                        players[event->targetID]->setMaxSpeed(CLERIC_MAX_SPEED);
                        break;
                    case ROGUE:
                        players[event->targetID]->setMaxSpeed(ROGUE_MAX_SPEED);
                        break;                
                    default:
                        break;
                }
                break;
            }
        case HEAL_END: {
                GameUpdate gameUpdate;
                gameUpdate.updateType = HEAL_BY_CLERIC_END;
                gameUpdate.id = event->targetID;
                addUpdate(gameUpdate);
                break;
            }
        case GAME_START: {
            this->started = true;

            // now it has started, we need to initialize all the players' positions and roles
            initPlayers();

            GameUpdate gameUpdate;
            gameUpdate.updateType = GAME_STARTED;
            addUpdate(gameUpdate);
            break;
        }
        default:
            break;
    }
}


/*
    Update events vector per server tick.
    If the currentTime has passed the event->time, execute the event and remove
    from the vector
*/
void Game::updateGameEvents () {
    std::vector<GameEvent*> newEvents;
    for (auto iter = events.begin(); iter != events.end(); iter++) {
        GameEvent* event = *iter;

        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> timeDiff = currentTime - event->time;
        // if current time has passed the expected trigger time, execute the event
        if (std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count() >= 0) {
            processEvent(event);
            delete event;
        } else {
            newEvents.push_back(event);
        }
    }

    events = newEvents;
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
        cout << "[" << players[i]->getTypeToString() << "] ";
        cout << "(" << players[i]->getPosition().x << ", ";
        cout << players[i]->getPosition().y << "), ";
        cout << "hp: " << players[i]->getHp();
        if (players[i]->isDead())
            cout << " (dead)";
        cout << "\n";
    }
    //initGameGrids();
    //printStats();
}

void Game::printSelectingScreen() {
    for(int i = 0; i < MAX_PLAYERS; i++) {
        printf("Player %d ", i);
        switch(idToJobType[i]) {
            case CLERIC:
                printf("has locked into CLERIC.\n");
                break;
            case FIGHTER:
                printf("has locked into FIGHTER.\n");
                break;
            case MAGE:
                printf("has locked into MAGE.\n");
                break;
            case ROGUE:
                printf("has locked into ROGUE.\n");
                break;
            case MONSTER:
                printf("has locked into MONSTER.\n");
                break;
            case UNKNOWN: 
                printf("has not locked in.\n");
                break;
            default:
                break;

        }
    }
    printf("========= Available Jobs ===========\n");
    for(auto job = availableJobs.begin(); job != availableJobs.end(); job++) {
        switch(*job) {
            case FIGHTER:
                printf("FIGHTER \t(Press 1 to lock in)\n");
                break;
            case MAGE:
                printf("MAGE    \t(Press 2 to lock in)\n");
                break;
            case CLERIC:
                printf("CLERIC  \t(Press 3 to lock in)\n");
                break;
            case ROGUE:
                printf("ROGUE   \t(Press 4 to lock in)\n");
                break;
            default:
                break;
        }
    }
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
            players[update.id]->hpDecrement(update.damageTaken, false);
            break;
        case PLAYER_HP_INCREMENT:
            players[update.id]->hpIncrement(update.healAmount);
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

        // Beacon Updates:
        case BEACON_BEING_TAKEN:
            printf("Beacon is being captured. (%f) \n", update.beaconCaptureAmount);
            break;
        case BEACON_DECAYING:
            printf("Beacon is being decaying. (%f) \n", update.beaconCaptureAmount);
            break;
        case BEACON_CAPTURED:
            printf("Beacon has been captured. \n");
            break;
        case BEACON_PING_PLAYER:
            printf("Enemy Player at %f, %f.\n", update.playerPos.x, update.playerPos.y);
            break;

        // Objectives:
        case HEAL_OBJECTIVE_TAKEN:
            players[update.id]->hpIncrement(update.healAmount);
            consumeObj((Objective *)gameGrids[update.gridPos.x][update.gridPos.y]);
            break;
        case ARMOR_OBJECTIVE_TAKEN:
            players[update.id]->setHp(players[update.id]->getHp() + update.healAmount);
            consumeObj((Objective *)gameGrids[update.gridPos.x][update.gridPos.y]);
            break;
        case EVO_OBJECTIVE_TAKEN:
            // The level up process done in another update.
            consumeObj((Objective *)gameGrids[update.gridPos.x][update.gridPos.y]);
            break;

        // Monster Levels Up
        case MONSTER_EVO_UP:
            ((Monster *)players[update.id])->setEvo(update.newEvoLevel);
            break;


        // Add Role Claims has to change player instances.
        case ROLE_CLAIMED:
            idToJobType[update.id] = update.roleClaimed;
            availableJobs.erase(update.roleClaimed);
            break;  

        // GAME_STARTED is sent by the server.
        case GAME_STARTED:
            this->started = true;
            initPlayers(); // need to init because client hasn't initialized players
            break;

        // Game End
        case GAME_END:
            if (update.endStatus == 1)
                printf("Game ends with status %d: hunters win!\n", update.endStatus);
            if (update.endStatus == 2)
                printf("Game ends with status %d: monster wins!\n", update.endStatus);
            if (update.endStatus == 3)
                printf("Game ends with status %d: tie!\n", update.endStatus);
            exit(0);
            break;
        default:
            printf("Not Handled Update Type: %d", update.updateType);
            break;
    }
}

void Game::consumeObj(Objective * obj) {
    GridPosition objPos = obj->getPosition();
    objectives.erase(std::find(objectives.begin(),objectives.end(), obj));
    delete gameGrids[objPos.x][objPos.y];
    gameGrids[objPos.x][objPos.y] = new Space(objPos);
}

void Game::checkEvoLevel() {
    // For the monster, updateEvo if they crossed the HP threshold
    for (auto i = 0; i < PLAYER_NUM; i++) {
        if (players[i]->getType() == MONSTER) {
            Monster * monster = (Monster*) players[i];
            //increase monster evo every server tick by MONSTER_EVO_TICK_INCREMENT
            monster->updateEvo(this, monster->getEvo() + MONSTER_EVO_TICK_INCREMENT);
            // printf("Monster evo level: ");
            // printf("%f\n", monster->getEvo());

            // Monster should force level for every chunk. However, the max evo should only be reached 
            // once the monster has reached critical (10%) HP. 
            if (monster->getHp() <= 0.1 * MONSTER_MAX_HP) { 
                if (monster->getEvo() < MONSTER_FIFTH_STAGE_THRESHOLD) {
                    monster->updateEvo(this, MONSTER_FIFTH_STAGE_THRESHOLD) ;
                }
            } else if (monster->getHp() <= 0.2 * MONSTER_MAX_HP) {
                if (monster->getEvo() < MONSTER_FOURTH_STAGE_THRESHOLD) {
                    monster->updateEvo(this, MONSTER_FOURTH_STAGE_THRESHOLD);
                }
            } else if (monster->getHp() <= 0.4 * MONSTER_MAX_HP) {
                if (monster->getEvo() < MONSTER_THIRD_STAGE_THRESHOLD) {
                    monster->updateEvo(this, MONSTER_THIRD_STAGE_THRESHOLD);
                }
            } else if (monster->getHp() <= 0.6 * MONSTER_MAX_HP) { 
                if (monster->getEvo() < MONSTER_SECOND_STAGE_THRESHOLD) {
                    monster->updateEvo(this, MONSTER_SECOND_STAGE_THRESHOLD);
                }
            } else if (monster->getHp() <= 0.8 * MONSTER_MAX_HP) { 
                if (monster->getEvo() < MONSTER_FIRST_STAGE_THRESHOLD) {
                    monster->updateEvo(this, MONSTER_FIRST_STAGE_THRESHOLD);
                }
            } else {
                return;
            }
        }
    } 
}

// testing  purposes
void Game::printStats() {

// <<<<<<< 
//     return;
    // int test_rock_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_rock_height = (MAP_HEIGHT / GRID_HEIGHT) / 2;

    // int test_fheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_fheal_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 1;

    // int test_mheal_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_mheal_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 2;
    
    // int test_mevo_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_mevo_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 3;

    // int test_farm_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_farm_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 4;

    // int test_beac_width = (MAP_WIDTH / GRID_WIDTH) / 2;
    // int test_beac_height = ((MAP_HEIGHT / GRID_WIDTH) / 2) - 5;

    // cout << "Fighter Heal amount - " << ((Heal*) gameGrids[test_fheal_height][test_fheal_width])->getHealAmount() << endl;
    // cout << "Monster Heal amount - " << ((Heal*) gameGrids[test_mheal_height][test_mheal_width])->getHealAmount() << endl;
    // cout << "Monster Evo amount - " << ((Evolve*) gameGrids[test_mevo_height][test_mevo_width])->getEvoAmount() << endl;
    // cout << "Fighter Armor amount - " << ((Armor*) gameGrids[test_farm_height][test_farm_width])->getArmorAmount() << endl;
    // cout << "Beacon Frequency (units unknown) - " << ((Beacon*) gameGrids[test_beac_height][test_beac_width])->getFrequency() << endl;   
//=======
    int test_rock_width = 30;
    int test_rock_height = 30;

    int test_fheal_width = 35;
    int test_fheal_height = 35;

    int test_mheal_width = 40;
    int test_mheal_height = 40;
    
    int test_mevo_width = 45;
    int test_mevo_height = 45;

    int test_farm_width = 50;
    int test_farm_height = 50;

    int test_beac_width = 55;
    int test_beac_height = 55;

    cout << "Fighter Heal amount - " << ((Heal*) gameGrids[test_fheal_height][test_fheal_width])->getHealAmount() << endl;
    cout << "Monster Heal amount - " << ((Heal*) gameGrids[test_mheal_height][test_mheal_width])->getHealAmount() << endl;
    cout << "Monster Evo amount - " << ((Evolve*) gameGrids[test_mevo_height][test_mevo_width])->getEvoAmount() << endl;
    cout << "Fighter Armor amount - " << ((Armor*) gameGrids[test_farm_height][test_farm_width])->getArmorAmount() << endl;
    cout << "Beacon Frequency (units unknown) - " << ((Beacon*) gameGrids[test_beac_height][test_beac_width])->getFrequency() << endl;   
}
