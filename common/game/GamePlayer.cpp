#include "GamePlayer.h"

GamePlayer::GamePlayer() {}

GamePlayer::GamePlayer(PlayerPosition position){
    type = UNKNOWN;
    specID = 0;
    prevTime = std::chrono::steady_clock::now();
    setFaceDirection(NORTH);
    setPosition(position);
    setSpeed(0); // init speed is 0
}

PlayerType GamePlayer::getType () { return type; }



const std::string GamePlayer::getTypeToString() {
    switch(this->type) {
        case CLERIC:   
            return "CLERIC";
        case ROGUE:
            return "ROGUE";
        case FIGHTER:
            return "FIGHTER";
        case MAGE:
            return "MAGE";  
        case MONSTER:
            return "MONSTER"; 
        default:
            return "UNKNOWN";
    }
}

void GamePlayer::setType (PlayerType newType) { type = newType; }

PlayerPosition GamePlayer::getPosition() { return position; }

void GamePlayer::setPosition (PlayerPosition newPosition) {
    position.x = newPosition.x;
    position.y = newPosition.y;
    position.width = newPosition.width;
    position.height = newPosition.height;
}

int GamePlayer::getMaxHp ()  { return maxHp; }

int GamePlayer::getHp ()  { return hp; }

void GamePlayer::setHp (int newHp) { hp = newHp; }

int GamePlayer::getAttackDamage ()  { return attackDamage;}

void GamePlayer::setAttackDamage (int newAttackDamage) { attackDamage = newAttackDamage; }

Direction GamePlayer::getFaceDirection() {return faceDirection; }

void GamePlayer::setFaceDirection(Direction newDirection) { 
    switch (newDirection)
    {
        case NORTH:
            faceDirection = NORTH;
            break;
        case EAST:
            faceDirection = EAST;
            break;
        case WEST:
            faceDirection = WEST;
            break;
        case SOUTH:
            faceDirection = SOUTH;
            break;
        case NORTH_EAST:
            faceDirection = EAST;
            break;
        case SOUTH_EAST:
            faceDirection = EAST;
            break;
        case NORTH_WEST:
            faceDirection = WEST;
            break;
        case SOUTH_WEST:
            faceDirection = WEST;
            break;  
        default:
            faceDirection = WEST;
            break;
    }
}

float GamePlayer::getSpeed() { return speed; }

void GamePlayer::setSpeed(float newSpeed) { speed = newSpeed; }

float GamePlayer::getAcceleration() { return acceleration; }

void GamePlayer::setAcceleration(float newAcceleration) { acceleration = newAcceleration; }

float GamePlayer::getMaxSpeed() { return maxSpeed; }

void GamePlayer::setMaxSpeed (float newMaxSpeed) {maxSpeed = newMaxSpeed;}

void GamePlayer::speedChange(float amount) {
    if (amount > 0) speedUp(amount);
    else slowDown(-1 * amount);
}

void GamePlayer::slowDown (float amount) { 
    float minSpeed = 0;
    speed = std::max(minSpeed, speed - amount); 
}

void GamePlayer::speedUp (float amount) { speed += amount; }


/*
    If isPlayer is true:
        According to the player's faceDirection, return upper left x coord 
        of player's bounding box
    Else:
        Simply return the upper left x coord of the position box
*/
float GamePlayer::getUpperLeftCoordinateX (PlayerPosition position, bool isPlayer) {
    if (!isPlayer) return position.x - position.width / 2; 

    if (faceDirection == NORTH || faceDirection == SOUTH)
        return position.x - position.width / 2;
    else
        return position.x - position.height / 2;
}

/*
    If isPlayer is true:
        According to the player's faceDirection, return upper left y coord 
        of player's bounding box
    Else:
        Simply return the upper left y coord of the position box
*/
float GamePlayer::getUpperLeftCoordinateY (PlayerPosition position, bool isPlayer) {
    if (!isPlayer) return position.y - position.height / 2; 

    if (faceDirection == NORTH || faceDirection == SOUTH)
        return position.y - position.height / 2;
    else
        return position.y - position.width / 2;
}

/*
    If isPlayer is true:
        According to the player's faceDirection, return bottom right y coord 
        of player's bounding box
    Else:
        Simply return the bottom right x coord of the position box
*/
float GamePlayer::getBottomRightCoordinateX (PlayerPosition position, bool isPlayer) {
    if (!isPlayer) return position.x + position.width / 2;

    if (faceDirection == NORTH || faceDirection == SOUTH)
        return position.x + position.width / 2;
    else
        return position.x + position.height / 2;
}

/*
    If isPlayer is true:
        According to the player's faceDirection, return bottom right y coord 
        of player's bounding box
    Else:
        Simply return the bottom right y coord of the position box
*/
float GamePlayer::getBottomRightCoordinateY (PlayerPosition position, bool isPlayer) {
    if (!isPlayer) return position.y + position.height / 2;

    if (faceDirection == NORTH || faceDirection == SOUTH)
        return position.y + position.height / 2;
    else
        return position.y + position.width / 2;
}

/*
    Return true is current player will collide the argument otherPlayer

    Note: If two players' edges overlap exactly, that is not a collision
          (Reason: bounding box will be slightly larger than character itself)
*/
bool GamePlayer::isCollidingPlayer (Game* game, PlayerPosition currentPosition) {
    float p1ULX = getUpperLeftCoordinateX(currentPosition, true);
    float p1ULY = getUpperLeftCoordinateY(currentPosition, true);
    float p1BRX = getBottomRightCoordinateX(currentPosition, true);
    float p1BRY = getBottomRightCoordinateY(currentPosition, true);

    for (int i = 0; i < PLAYER_NUM; i++) {
        // skip the player itself
        if (game->players[i] == this) continue;
        // skip dead players
        if (game->players[i]->isDead()) continue;
        GamePlayer* otherPlayer = game->players[i];
        float p2ULX = getUpperLeftCoordinateX(otherPlayer->position, true);
        float p2ULY = getUpperLeftCoordinateY(otherPlayer->position, true);
        float p2BRX = getBottomRightCoordinateX(otherPlayer->position, true);
        float p2BRY = getBottomRightCoordinateY(otherPlayer->position, true);

        // https://www.geeksforgeeks.org/find-two-rectangles-overlap/
        // if one box is on left side of other
        if (p1ULX >= p2BRX || p2ULX >= p1BRX) continue;
        // if one box is above the other
        if (p1ULY >= p2BRY || p2ULY >= p1BRY) continue;

        return true;
    }

    return false;
}

/*
    Return true if the current player is colliding any obstacles in the game grids

    Note: We are expecting GridSize to be much smaller than player bounding box
          to achieve finer art assets
    Kinda like:

                -------------
                |gggggggggggg|
                |gggggggggggg|
                --------------
*/
bool GamePlayer::isCollidingObstacle (Game* game, PlayerPosition currentPosition) {
    float ulX = getUpperLeftCoordinateX(currentPosition, true);
    float ulY = getUpperLeftCoordinateY(currentPosition, true);
    int xStartIndex = int(ulX / GRID_WIDTH);
    int yStartIndex = int(ulY / GRID_HEIGHT);

    float brX = getBottomRightCoordinateX(currentPosition, true);
    float brY = getBottomRightCoordinateY(currentPosition, true);
    int xEndIndex = int(brX / GRID_WIDTH);
    int yEndIndex = int(brY / GRID_HEIGHT);

    for (int i = xStartIndex; i <= xEndIndex; i++) {
        for (int j = yStartIndex; j <= yEndIndex; j++) {
            if (game->gameGrids[i][j]->isObstacle()) return true;
        }
    }

    return false;
}

/*
    Return true, if the player can move to the argument position

    Note: This canMove cannot check obstacles on the way, it only check whether collisions
          will happen at the destination.

          This is more like a unit move where player will only be moved a short distance.
*/
bool GamePlayer::canMoveTo(Game* game, PlayerPosition position) {
    // if destination position is out of map, cannot move
    if (getUpperLeftCoordinateX(position, true) < 0 || 
                                        getUpperLeftCoordinateY(position, true) < 0) {
        return false;
    }
    if (getBottomRightCoordinateX(position, true) > MAP_WIDTH - 1 || 
                        getBottomRightCoordinateY(position, true) > MAP_HEIGHT - 1) {
        return false;
    }

    // if destination position collides other players, cannot move
    if (isCollidingPlayer(game, position)) return false;

    // if destination position collides obstacles, cannot move
    if (isCollidingObstacle(game, position)) return false;

    return true;
}

/*
    Return true if the argument player is attackable for this player.

    Current Rule: players can attack each other if they are enemies
*/
bool GamePlayer::canAttack (GamePlayer* player) {
    if (player->getType() == MONSTER && type != MONSTER) return true;
    if (type == MONSTER && player->getType() != MONSTER) return true;

    return false;
}


/*
    Return true if p1 and p2 are the same
*/
bool GamePlayer::samePosition (PlayerPosition p1, PlayerPosition p2) {
    if (abs(p1.x - p2.x) > DELTA) return false;
    if (abs(p1.y - p2.y) > DELTA) return false;
    if (abs(p1.width - p2.width) > DELTA) return false;
    if (abs(p1.height - p2.height) > DELTA) return false;
    return true;
}

bool clearSpeed (Direction faceDirection, Direction moveDirection) {
    switch (faceDirection)
    {
        case NORTH:
            return !(moveDirection == NORTH || moveDirection == NORTH_EAST || moveDirection == NORTH_WEST);
            break;
        case EAST:
            return !(moveDirection == EAST || moveDirection == NORTH_EAST || moveDirection == SOUTH_EAST);
            break;
        case SOUTH:
            return !(moveDirection == SOUTH || moveDirection == SOUTH_WEST || moveDirection == SOUTH_EAST);
            break;
        case WEST:
            return !(moveDirection == WEST || moveDirection == SOUTH_WEST || moveDirection == NORTH_WEST);
            break;
        case NORTH_WEST:
            return !(moveDirection == WEST || moveDirection == NORTH_WEST || moveDirection == NORTH);
            break;
        case NORTH_EAST:
            return !(moveDirection == NORTH || moveDirection == NORTH_EAST || moveDirection == EAST);
            break;
        case SOUTH_EAST:
            return !(moveDirection == EAST || moveDirection == SOUTH_EAST || moveDirection == SOUTH);
            break;
        case SOUTH_WEST:
            return !(moveDirection == SOUTH || moveDirection == SOUTH_WEST || moveDirection == WEST);
            break;
        default:
            return true;
            break;
    }
}



/* 
    Move the player one grid to the specified direction if possible.
    Set player faceDirection to the direction parameter regardless whether
    the movement is mode successfully or not.
    
    We assume player position is valid here (player position does not go beyond map)
*/
void GamePlayer::move (Game* game, Direction direction) {
    // cancel prescheduled "speed goes back to 0" event
    std::vector<GameEvent*> newEvents;
    for (auto iter = game->events.begin(); iter != game->events.end(); iter++) {
        GameEvent* event = *iter;
        if (event->ownerID == getID() && event->targetID == getID() && event->type == SPEED_CHANGE) delete event;
        else newEvents.push_back(event);
    }   
    game->events = newEvents;   

    setFaceDirection(direction);
    

    // if player changes direction, set speed back to 0
    // if (clearSpeed(getFaceDirection(), direction)) speed = 0;
    // else 
    speed = std::min(speed + acceleration, maxSpeed);

    // printf("direction: %d, facedirection: %d, speed: %f\n", direction, faceDirection, speed);


    // turn the face direction as the parameter direction no matter the movement is succ or not
    // setFaceDirection(direction);

    // calculating destination
    PlayerPosition destPosition = PlayerPosition();
    destPosition.width = position.width;
    destPosition.height = position.height;

    // x stays the same
    if (direction == NORTH || direction == SOUTH) {
        destPosition.x = position.x;
        destPosition.y = direction == NORTH ? position.y - speed : position.y + speed;
    } else if (direction == EAST || direction == WEST) {
    // y stays the same
        destPosition.y = position.y;
        destPosition.x = direction == WEST ? position.x - speed : position.x + speed;
    } else if (direction == NORTH_EAST || direction == NORTH_WEST) {
        destPosition.y = position.y - speed/sqrt(2);
        destPosition.x = direction == NORTH_WEST ? position.x - speed/sqrt(2) : position.x + speed/sqrt(2);
    } else if (direction == SOUTH_EAST || direction == SOUTH_WEST) {
        destPosition.y = position.y + speed/sqrt(2);
        destPosition.x = direction == SOUTH_WEST ? position.x - speed/sqrt(2) : position.x + speed/sqrt(2);
    }

    // if destination is invalid, return immediately
    if (!canMoveTo(game, destPosition)) {
        // player destination invalid, change speed to 0
        speed = 0;
        return;
    }

    // schedule a "speed back to 0" event, if there is no consecutive move, acceleration process terminate
    GameEvent* event = new GameEvent();
    event->type = SPEED_CHANGE;
    event->ownerID = getID();
    event->targetID = getID();
    event->amount = -1 * speed;
    event->time = std::chrono::steady_clock::now() + std::chrono::milliseconds(TICK_TIME);
    game->events.push_back(event);

    // push update onto queue for clients to know that a player has moved
    GameUpdate gameUpdate;
    gameUpdate.id = this->id;
    gameUpdate.updateType = PLAYER_MOVE;
    gameUpdate.floatDeltaX = destPosition.x - position.x;
    gameUpdate.floatDeltaY = destPosition.y - position.y;
    gameUpdate.direction = getFaceDirection();
    game->addUpdate(gameUpdate);

    // Move there!
    position = destPosition;
}


void GamePlayer::hpDecrement (int damage) {
    if (hp == 0) return;
    hp = std::max(0, hp - damage);
    if (hp == 0) {
        deathTime = std::chrono::steady_clock::now();
    }
}

void GamePlayer::hpIncrement (int amount) {
    hp = std::min(maxHp, hp + amount);
}

bool GamePlayer::isDead () {
    return hp <= 0;
}

void GamePlayer::attack(Game* game, float angle) {
    printf("Overwriten failed\n");
}

void GamePlayer::uniqueAttack(Game* game, float angle) {
    // printf("Default second attack\n");
}

// Interact goes through the possible objectives and tries to interact with nearby objective
// I made this a virtual method because Evolve obj requires accessing monster's evo level.
void GamePlayer::interact(Game* game) {
    printf("Overridden Method failed.\n");
}

PlayerPosition GamePlayer::bfsSearchNonCollidingPosition(Game* game) {
    PlayerPosition newPosition = this->position;
    std::queue<GridPosition> positionQueue;
    std::vector<GridPosition> used;
    while(this->isCollidingObstacle(game, newPosition) || this->isCollidingPlayer(game, newPosition)) {
        // Gridposition used instead of player position because floats might mess up the calculations
        GridPosition moveRight = {(int) (newPosition.x + newPosition.width), (int) (newPosition.y)};
        GridPosition moveLeft = {(int) (newPosition.x - newPosition.width), (int) (newPosition.y)};
        GridPosition moveUp = {(int) (newPosition.x), (int) (newPosition.y - newPosition.height)};
        GridPosition moveDown = {(int) (newPosition.x), (int) (newPosition.y - newPosition.height)};
        if(std::find(used.begin(), used.end(), moveRight) == used.end()) {
            positionQueue.push(moveRight);
            used.push_back(moveRight);
        }
        if(std::find(used.begin(), used.end(), moveRight) == used.end()) {
            positionQueue.push(moveRight);
            used.push_back(moveRight);
        }
        if(std::find(used.begin(), used.end(), moveLeft) == used.end()) {
            positionQueue.push(moveLeft);
            used.push_back(moveLeft);
        }
        if(std::find(used.begin(), used.end(), moveUp) == used.end()) {
            positionQueue.push(moveUp);
            used.push_back(moveUp);
        }
        if(std::find(used.begin(), used.end(), moveDown) == used.end()) {
            positionQueue.push(moveDown);
            used.push_back(moveDown);
        }
        if(!positionQueue.empty()) {
            GridPosition position = positionQueue.front();
            newPosition.x = position.x;
            newPosition.y = position.y;
            positionQueue.pop();
        }
    }
    return newPosition;
}

// Will revive a dead hunter
void GamePlayer::revive(Game* game) {

    if (this->getType() == MONSTER) {
        return;
    }

    for (int i = 0; i < PLAYER_NUM; i++) {
        // skip the player itself
        if (game->players[i] == this) continue;
        if (game->players[i]->isDead()) {
            GamePlayer* otherPlayer = game->players[i];
            auto currentTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> duration = currentTime - otherPlayer->deathTime;
            printf("before checking time\n");
            if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() 
                                            <= REVIVE_TIME_INTERVAL) {
                continue;
            }

            int distance = sqrt(pow((this->getPosition().x - otherPlayer->getPosition().x),2) 
             + pow((this->getPosition().y - otherPlayer->getPosition().y),2));
            
            if (distance <= REVIVE_DISTANCE) {
                // only want to increase HP of other hunters 
                if (!canAttack(game->players[i])) {
                    int maxHp = game->players[i]->getMaxHp();
                    game->players[i]->hpIncrement(maxHp/2);

                    // if the current player is colliding with the revived player, just move it elsewhere.
                    if(this->isCollidingPlayer(game, this->position)) {
                        PlayerPosition newPosition = bfsSearchNonCollidingPosition(game);
                        
                        // push update onto queue for clients to know that a player has been unstuck
                        GameUpdate gameUpdate;
                        gameUpdate.id = this->id;
                        gameUpdate.updateType = PLAYER_MOVE;
                        gameUpdate.floatDeltaX = newPosition.x - position.x;
                        gameUpdate.floatDeltaY = newPosition.y - position.y;
                        gameUpdate.direction = getFaceDirection();
                        game->addUpdate(gameUpdate);

                        // Move to the new position
                        position = newPosition;
                    }

                    // queue this update to be sent to other players
                    GameUpdate gameUpdate;
                    gameUpdate.updateType = PLAYER_REVIVE;
                    gameUpdate.id = i;
                    gameUpdate.healAmount = maxHp/2;
                    game->addUpdate(gameUpdate);
                }  
            }
        } 
    }
}

// Interacts with a Healing Objective
void GamePlayer::interactHeal(Game* game, Heal * healObj) {
    if(this->hp >= maxHp) {
        printf("Player (%d): Full HP, do not consume objective.\n", this->id);
        return;
    }
    int oldHp = this->getHp();
    int healAmount = healObj->getHealAmount();
    hpIncrement(healAmount);

    // Send an update to the clients: HEALING_OBJECTIVE_TAKEN
    GameUpdate healingUpdate;
    healingUpdate.updateType = HEAL_OBJECTIVE_TAKEN;
    healingUpdate.id = this->id;                        // id of player being healed
    healingUpdate.objectiveID = healObj->getObjectiveID();
    healingUpdate.healAmount = this->getHp() - oldHp;              // healed amount
    healingUpdate.gridPos = healObj->getPosition();     // obj location
    game->addUpdate(healingUpdate);

    // Clean up the healing grid.
    game->consumeObj(healObj);
}

// Interacts with an Armor Objective
void GamePlayer::interactArmor(Game * game, Armor * armorObj) {
    
    int oldHp = this->getHp();
    int healAmount = armorObj->getArmorAmount();
    this->setHp(oldHp + healAmount);

    // Send an update to the clients: ARMOR_OBJECTIVE_TAKEN
    GameUpdate armorUpdate;
    armorUpdate.updateType = ARMOR_OBJECTIVE_TAKEN;
    armorUpdate.id = this->id;                            // id of player being healed
    armorUpdate.objectiveID = armorObj->getObjectiveID();
    armorUpdate.healAmount = this->getHp() - oldHp;  // healed amount
    armorUpdate.gridPos = armorObj->getPosition();        // obj location
    game->addUpdate(armorUpdate);

    // Clean up the healing grid.
    game->consumeObj(armorObj);
}

// Check if the player is within the range of an objective
bool GamePlayer::isWithinObjective(Objective * objective) {

    // Should not overflow because the max distance a player can be from objective is 600^2 or 360,000
    GridPosition objectivePos = objective->getPosition();
    float squaredDistanceX =  pow(this->position.x - objectivePos.x, 2);
    float squaredDistanceY = pow(this->position.y - objectivePos.y, 2);

    // squared distance used instead of distance because less computation required.
    return squaredDistanceX + squaredDistanceY <= pow(objective->getInteractionRange(), 2);
}

// Check if player's type is valid to interact with the objective
bool GamePlayer::canInteractWithObjective(Objective * objective) {

    // if it's a beacon then the player didn't need to press E on it.
    if(objective->getObjective() == BEACON) {
        return false;
    }

    // If the objective is for the monster or neutral, then a monster can interact it.
    if(this->getType() == MONSTER && (objective->getRestriction() == R_MONSTER || objective->getRestriction() == R_NEUTRAL)) {
        return true;

    // If this objective is for non-monsters / non-unknowns (hunters) and the 
    // restriction is not monsters, then the hunters can interact with it.
    } else if (this->getType() != MONSTER && this->getType() != UNKNOWN && objective->getRestriction() != R_MONSTER) {
        return true;
    }
            

    return false;
    
}

long long GamePlayer::getTimeDiff() {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> timeDiff = currentTime - prevTime;
    return std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count();
}

void GamePlayer::spectate(Game* game, UPDATE_TYPE update) {
    
    if(getTimeDiff() <= SPECTATE_INPUT_DELAY) return;

    GameUpdate gameUpdate;
    int numLooped = 0;

    // find the next available to player to spectate
    // a player can be spectated if all conditions are satisfied: 
    // (1) it's not themselves, (2) it's not the monster, (3) the other player is alive
    do {
        if(update == PLAYER_NEXT_SPECT)
            this->specID = (this->specID + 1) % (MAX_PLAYERS - 1); // skip monster ID
        else {
            this->specID--;
            if(this->specID < 0) 
                this->specID = MAX_PLAYERS - 2; // skip monster ID
        }
        printf("Loop: Spectator id %d\n", this->specID);
        ++numLooped;
    } while(numLooped < MAX_PLAYERS - 1 && (this->specID == this->id || game->players[specID]->isDead()));

    gameUpdate.id = this->id;
    gameUpdate.updateType = update;
    gameUpdate.specID = this->specID;
    game->addUpdate(gameUpdate);
    prevTime = std::chrono::steady_clock::now();
    printf("Spectator id %d\n", this->specID);
}

bool allowLeftMouseShooting(PlayerType type) {
    return type == MAGE || type == CLERIC || type == ROGUE;
}

bool allowRightMouseShooting(PlayerType type) {
    return type == MAGE || type == ROGUE;
}

void GamePlayer::handleUserInput (Game* game, GAME_INPUT userInput) {
    // if player is dead, stop handling input
    if (isDead()) {
        switch(userInput.input) {
            case LEFT_MOUSE_ATTACK:
                spectate(game, PLAYER_NEXT_SPECT);
                break;
            case RIGHT_MOUSE_ATTACK:
                spectate(game, PLAYER_PREV_SPECT);
                break;
        }
        return;
    }

    switch (userInput.input) {
        // straight movement
        case MOVE_FORWARD:
            move(game, NORTH);
            break;
        case MOVE_BACKWARD:
            move(game, SOUTH);
            break;
        case MOVE_LEFT:
            move(game, WEST);
            break;
        case MOVE_RIGHT:
            move(game, EAST);
            break;
        // diagonal movement
        case MOVE_UPLEFT:
            move(game, NORTH_WEST);
            break;
        case MOVE_UPRIGHT:
            move(game, NORTH_EAST);
            break;
        case MOVE_DOWNLEFT:
            move(game, SOUTH_WEST);
            break;
        case MOVE_DOWNRIGHT:
            move(game, SOUTH_EAST);
            break;
        // straight move and attack
        case MOVE_FORWARD_ATTACK:
            move(game, NORTH);
            attack(game, userInput.angle);
            break;
        case MOVE_FORWARD_UNIQUE_ATTACK:
            move(game, NORTH);
            uniqueAttack(game, userInput.angle);
            break;
        case MOVE_BACKWARD_ATTACK:
            move(game, SOUTH);
            attack(game, userInput.angle);
            break;
        case MOVE_BACKWARD_UNIQUE_ATTACK:
            move(game, SOUTH);
            uniqueAttack(game, userInput.angle);
            break;
        case MOVE_LEFT_ATTACK:
            move(game, WEST);
            attack(game, userInput.angle);
            break;
        case MOVE_LEFT_UNIQUE_ATTACK:
            move(game, WEST);
            uniqueAttack(game, userInput.angle);
            break;
        case MOVE_RIGHT_ATTACK:
            move(game, EAST);
            attack(game, userInput.angle);
            break;
        case MOVE_RIGHT_UNIQUE_ATTACK:
            move(game, EAST);
            uniqueAttack(game, userInput.angle);
            break;
        // diagonal move and attack
        case MOVE_UPLEFT_ATTACK:
            move(game, NORTH_WEST);
            attack(game, userInput.angle);
            break;
        case MOVE_UPLEFT_UNIQUE_ATTACK:
            move(game, NORTH_WEST);
            uniqueAttack(game, userInput.angle);
            break;
        case MOVE_UPRIGHT_ATTACK:
            move(game, NORTH_EAST);
            attack(game, userInput.angle);
            break;
        case MOVE_UPRIGHT_UNIQUE_ATTACK:
            move(game, NORTH_EAST);
            uniqueAttack(game, userInput.angle);
            break;
        case MOVE_DOWNRIGHT_ATTACK:
            move(game, SOUTH_EAST);
            attack(game, userInput.angle);
            break;
        case MOVE_DOWNRIGHT_UNIQUE_ATTACK:
            move(game, SOUTH_EAST);
            uniqueAttack(game, userInput.angle);
            break;
        case MOVE_DOWNLEFT_ATTACK:
            move(game, SOUTH_WEST);
            attack(game, userInput.angle);
            break;
        case MOVE_DOWNLEFT_UNIQUE_ATTACK:
            move(game, SOUTH_WEST);
            uniqueAttack(game, userInput.angle);
            break;
        case INTERACT:
            interact(game);
            break;
        case REVIVE:
            revive(game);
            break;
        case LEFT_MOUSE_ATTACK:
            attack(game, userInput.angle);
            break;
        case RIGHT_MOUSE_ATTACK:
            uniqueAttack(game, userInput.angle);
            break;
        default:
            // NO_MOVE and other input does not trigger any action
            break;
    }
}

void GamePlayer::setID(int newID) {
    id = newID;
}

int GamePlayer::getID() {
    return id;
}