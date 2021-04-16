#include "GamePlayer.h"

GamePlayer::GamePlayer() {}

GamePlayer::GamePlayer(ComponentPosition position) {
    setPosition(position.x, position.y); 
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
    Move the player one grid to the specified direction if possible.
    Set player faceDirection to the direction parameter regardless whether
    the movement is mode successfully or not.
    
    We assume player position is valid here (player position does not go beyond map)
*/
void GamePlayer::move (Game* game, Direction direction) {
    int x = getPosition().x;
    int y = getPosition().y;

    // turn the face direction as the parameter direction no matter the movement is succ or not
    setFaceDirection(direction);

    // get the component at the targeted grid
    GameComponent* targetComponent = game->getGameComponentInDirection(x, y, direction);

    
    // if the targeted grid is valid and an empty space, move there!
    if (targetComponent != NULL && targetComponent->isSpace()) {
        game->swapGameComponents(targetComponent, game->gameGrids[x][y]);
    }
}


void GamePlayer::hpDecrement (int amount) {
    hp -= amount;
}

bool GamePlayer::isDead () {
    return hp <= 0;
}

/*
    Return true if player's current position is invalid
    Player's position is invalid if
    - Player is out of map
    OR
    - Plaayer is sitting on boundary

    FUTURE WORK:
    We can reset player's position if current position is invalid,
    this is to prevent system crash because of index error.
*/
bool GamePlayer::invalidPlayerPosition () {
    int x = getPosition().x;
    int y = getPosition().y;

    return x <= 0 || x >= MAP_WIDTH || y <= 0 || y >= MAP_HEIGHT;
}

void GamePlayer::attack(Game* game) {
    int x = getPosition().x;
    int y = getPosition().y;

    // if player's position is invalid, return directly
    if (invalidPlayerPosition()) return;

    GameComponent* targetComponent = game->getGameComponentInDirection(x, y, faceDirection);

    if (targetComponent->isMonster()) {
        ((GamePlayer*) targetComponent)->hpDecrement(attackHarm);
    }
}

void GamePlayer::handleUserInput (Game* game, CLIENT_INPUT userInput) {
    switch (userInput) {
        case MOVE_FORWARD:
            // game->addUpdate(PLAYER_MOVE, 0, -1, 0, -.25f, 0.f);
            move(game, NORTH);
            break;
        case MOVE_BACKWARD:
            // game->addUpdate(PLAYER_MOVE, 0, 1, 0, .25f, 0);
            move(game, SOUTH);
            break;
        case MOVE_LEFT:
            // game->addUpdate(PLAYER_MOVE, 0, 0, -1, 0.f, -.25f);
            move(game, WEST);
            break;
        case MOVE_RIGHT:
            // game->addUpdate(PLAYER_MOVE, 0, 0, 1, 0.f, .25f);
            move(game, EAST);
            break;
        case ATTACK:
            // game->addUpdate(PLAYER_DAMAGE_TAKEN, 0, 0, 0, 0.f, 0.f);
            attack(game);
            break;
        default:
            // NO_MOVE and other input does not trigger any action
            break;
    }
}
