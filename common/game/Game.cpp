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
    // initPlayers(); Will init players when the game starts and we know the jobs
    initSelectScreenStructures();

    // Give players SELECT_SCREEN_TIME seconds to select jobs
    GameEvent* gameStartEvent = new GameEvent();
    gameStartEvent->type = SPEED_CHANGE;
    gameStartEvent->time = std::chrono::steady_clock::now() + 
                    std::chrono::seconds(SELECT_SCREEN_TIME);
    this->events.push_back(gameStartEvent);
}

// initializes the map structure for easier selecting of jobs
void Game::initSelectScreenStructures() {
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

    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i]->setID(i);
        
        PlayerPosition position = PlayerPosition();
        position.x = SPAWN_POSITIONS[i][0];
        position.y = SPAWN_POSITIONS[i][1];
        bool isUnknown = false;
        switch(idToJobType[i]) {
            case CLERIC:
                position.width = CLERIC_WIDTH;
                position.height = CLERIC_HEIGHT;
                players[i] = new Cleric(position);
                break;
            case FIGHTER:
                position.width = FIGHTER_WIDTH;
                position.height = FIGHTER_HEIGHT;
                players[i] = new Fighter(position);
                break;
            case MAGE:
                position.width = MAGE_WIDTH;
                position.height = MAGE_HEIGHT;
                players[i] = new Mage(position);
                break;
            case ROGUE:
                position.width = ROGUE_WIDTH;
                position.height = ROGUE_HEIGHT;
                players[i] = new Rogue(position);
                break;
            case MONSTER:
                position.width = MONSTER_WIDTH;
                position.height = MONSTER_HEIGHT;
                players[i] = new Monster(position);
                break;
            case UNKNOWN: {
                PlayerType availableJob = *availableJobs.begin();
                idToJobType[i] = availableJob;
                availableJobs.erase(availableJob);
                i--; // decrement to restart the process for this player
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

    for (int i = 0; i < MAP_HEIGHT / GRID_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH / GRID_WIDTH; j++) {
            GridPosition position;
            position.x = j;
            position.y = i;
            // put obstacles on the boundary
            if (isBoundary(j, i)) {
                gameGrids[i][j] = new Obstacle(position);
            }
            else {
                
                // testing purposes
                if(j == test_rock_width && i == test_rock_height) {
                    gameGrids[i][j] = new Rock(position);
                }
                else if (j == test_fheal_width && i == test_fheal_height) {
                    gameGrids[i][j] = new Heal(position, R_HUNTER);
                    objectives.push_back((Objective *)gameGrids[i][j]);
                }
                else if (j == test_mheal_width && i == test_mheal_height) {
                    gameGrids[i][j] = new Heal(position, R_MONSTER);
                    objectives.push_back((Objective *)gameGrids[i][j]);
                }
                else if (j == test_mevo_width && i == test_mevo_height) {
                    gameGrids[i][j] = new Evolve(position);
                    objectives.push_back((Objective *)gameGrids[i][j]);
                }
                else if (j == test_farm_width && i == test_farm_height) {
                    gameGrids[i][j] = new Armor(position, R_HUNTER);
                    objectives.push_back((Objective *)gameGrids[i][j]);
                }
                else if (j == test_beac_width && i == test_beac_height) {
                    gameGrids[i][j] = new Beacon(position);
                    beacon = (Beacon*) gameGrids[i][j];
                }
                else {
                    gameGrids[i][j] = new Space(position);
                }

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
        GamePlayer* otherPlayer = game->players[i];

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

                // create an event to add the speed back later
                GameEvent* event = new GameEvent();
                event->type = SPEED_CHANGE;
                event->ownerID = p->ownerID;
                event->targetID = i;
                event->amount = FIREBALL_SPEED_DEC;
                event->time = std::chrono::steady_clock::now() + 
                                std::chrono::milliseconds(FIREBALL_EFFECT_TIME);
                game->events.push_back(event);
            }
            // projectile will only cause damage 
            else {
                otherPlayer->hpDecrement(p->damage);

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
        } else if (beacon->getCaptureAmount() < -1.f || beacon->getCaptureAmount() > 1.f) {
            beacon->decayCaptureAmount();
            
            // send delaying update to all players
            GameUpdate beaconDecayingUpdate;
            beaconDecayingUpdate.updateType = BEACON_DECAYING;
            beaconDecayingUpdate.beaconCaptureAmount = beacon->getCaptureAmount();
            this->addUpdate(beaconDecayingUpdate);

        }
        
    }
}

/* Process a single event */
void Game::processEvent (GameEvent* event) {
    switch (event->type)
    {
        case HP_DEC: {
                players[event->targetID]->hpDecrement(event->amount);
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
            break;
        case GAME_START:
            this->started = true;

            // now it has started, we need to initialize all the players' positions and roles
            initPlayers();
            break;
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
        case PLAYER_HP_INCREMENT:
            players[update.id]->hpIncrement(update.damageTaken);
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


        // Add Role Claims
        // TODO: add role claims -> has to change player instances.

        default:
            printf("Not Handled Update Type: %d", update.updateType);
            break;
    }
}
// testing  purposes
void Game::printStats() {

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

void Game::consumeObj(Objective * obj) {
    GridPosition objPos = obj->getPosition();
    objectives.erase(std::find(objectives.begin(),objectives.end(), obj));
    delete gameGrids[objPos.x][objPos.y];
    gameGrids[objPos.x][objPos.y] = new Space(objPos);
}