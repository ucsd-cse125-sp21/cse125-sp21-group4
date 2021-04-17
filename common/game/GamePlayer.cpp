#include "GamePlayer.h"

GamePlayer::GamePlayer() {}

GamePlayer::GamePlayer(PlayerPosition position) {
    type = UNKNOWN;
    setPosition(position);
}

PlayerType GamePlayer::getType () { return type; }

void GamePlayer::setType (PlayerType newType) { type = newType; }

PlayerPosition GamePlayer::getPosition() { return position; }

void GamePlayer::setPosition (PlayerPosition newPosition) {
    position.x = newPosition.x;
    position.y = newPosition.y;
    position.width = newPosition.width;
    position.height = newPosition.height;
}

int GamePlayer::getHp ()  { return hp; }

void GamePlayer::setHp (int newHp) { hp = newHp; }

int GamePlayer::getAttackHarm ()  { return attackHarm;}

void GamePlayer::setAttackHarm (int newAttackHarm) { attackHarm = newAttackHarm; }

Direction GamePlayer::getFaceDirection() {return faceDirection; }

void GamePlayer::setFaceDirection(Direction newDirection) { 
    faceDirection = newDirection; 
}

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
    Move the player one grid to the specified direction if possible.
    Set player faceDirection to the direction parameter regardless whether
    the movement is mode successfully or not.
    
    We assume player position is valid here (player position does not go beyond map)
*/
void GamePlayer::move (Game* game, Direction direction, float distance) {

    // turn the face direction as the parameter direction no matter the movement is succ or not
    setFaceDirection(direction);

    // calculating destination
    PlayerPosition destPosition = PlayerPosition();
    destPosition.width = position.width;
    destPosition.height = position.height;

    // x stays the same
    if (direction == NORTH || direction == SOUTH) {
        destPosition.x = position.x;
        destPosition.y = direction == NORTH ? position.y - distance : position.y + distance;
    } else {
    // y stays the same
        destPosition.y = position.y;
        destPosition.x = direction == WEST ? position.x - distance : position.x + distance;
    }

    // if destination is invalid, return immediately
    if (!canMoveTo(game, destPosition)) return;

    // Move there!
    position = destPosition;
}


void GamePlayer::hpDecrement (int amount) {
    hp -= amount;
}

bool GamePlayer::isDead () {
    return hp <= 0;
}

/*
    General attack function
    Player can attack the enemy whose bounding box has overlapped with the
    following region.
        ----------
        | attack | distance
        ---------
        |Bounding|
        | Box    | player height
        ----------
        Player width
*/
void GamePlayer::attack(Game* game, float distance) {
    // draw the attack region
    PlayerPosition attackRegion = PlayerPosition();
    if (faceDirection == NORTH || faceDirection == SOUTH) {
        attackRegion.x = position.x;
        attackRegion.width = position.width;
        attackRegion.height = distance;
        if (faceDirection == NORTH)
            attackRegion.y = position.y - position.height/2 - distance / 2;
        else
            attackRegion.y = position.y + position.height/2 + distance / 2;
    } else {
        attackRegion.y = position.y;
        attackRegion.width = distance;
        attackRegion.height = position.width;
        if (faceDirection = EAST)
            attackRegion.x = position.x + position.height / 2 + distance / 2;
        else
            attackRegion.x = position.x - position.height / 2 - distance / 2;
    }

    // for every player, if their bounding box overlaps the attackRegion, and
    // they are enemies of this player, decrement their hp
    float p1ULX = getUpperLeftCoordinateX(attackRegion, false);
    float p1ULY = getUpperLeftCoordinateY(attackRegion, false);
    float p1BRX = getBottomRightCoordinateX(attackRegion, false);
    float p1BRY = getBottomRightCoordinateY(attackRegion, false);

    for (int i = 0; i < PLAYER_NUM; i++) {
        // skip the player itself
        if (game->players[i] == this) continue;
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

        if (canAttack(game->players[i]))
            game->players[i]->hpDecrement(attackHarm);
    }
}

void GamePlayer::handleUserInput (Game* game, CLIENT_INPUT userInput) {
    switch (userInput) {
        // ERIC todo: add updates
        case MOVE_FORWARD:
            move(game, NORTH, MOVE_DISTANCE);
            break;
        case MOVE_BACKWARD:
            move(game, SOUTH, MOVE_DISTANCE);
            break;
        case MOVE_LEFT:
            move(game, WEST, MOVE_DISTANCE);
            break;
        case MOVE_RIGHT:
            move(game, EAST, MOVE_DISTANCE);
            break;
        case ATTACK:
            attack(game, ATTACK_DISTANCE);
            break;
        default:
            // NO_MOVE and other input does not trigger any action
            break;
    }
}
