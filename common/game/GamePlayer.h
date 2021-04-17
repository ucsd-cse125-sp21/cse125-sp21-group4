#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_
#include "GameConstants.h"
#include "Game.h"

// A GamePlayer's Bouding Box looks like
//          width
//     --------------
//     |            |
//     |            |
//     |    (x,y)   | height
//     |            |
//     |            |
//     --------------

struct PlayerPosition {
    float x; // x coordinate of center
    float y; // y coordinate of center
    float width; // width of the bounding box
    float height; // height of the bounding box
};
typedef PlayerPosition PlayerPosition;

enum PlayerType {
    UNKNOWN,
    MONSTER,
    FIGHTER,
};


class GamePlayer {
protected:
    PlayerType type;
    PlayerPosition position; // position struct for player
    int hp; // health for GamePlayer
    int attackHarm; // harmness made per attack
    Direction faceDirection; // the direction the player is facing

public:
    GamePlayer(); // Constructor for GamePlayer
    GamePlayer(PlayerPosition position); // Constructor for GameComponent

    PlayerType getType();
    void setType(PlayerType newType);
    PlayerPosition getPosition();
    void setPosition(PlayerPosition newPosition);
    int getHp ();
    void setHp (int newHp);
    int getAttackHarm ();
    void setAttackHarm (int newAttackHarm);
    Direction getFaceDirection (); // faceDirection getter
    void setFaceDirection (Direction newDirection); // faceDirection setter

    float getUpperLeftCoordinateX (PlayerPosition position, bool isPlayer);
    float getUpperLeftCoordinateY (PlayerPosition position, bool isPlayer);
    float getBottomRightCoordinateX (PlayerPosition position, bool isPlayer);
    float getBottomRightCoordinateY (PlayerPosition position, bool isPlayer);

    bool isCollidingPlayer(Game* game, PlayerPosition currentPosition);
    bool isCollidingObstacle(Game* game, PlayerPosition currentPosition);
    bool canMoveTo(Game* game, PlayerPosition position);
    bool canAttack (GamePlayer* player);

    void move (Game* game, Direction direction, float distance); 
    void attack (Game* game, float distance);
    void hpDecrement (int amount);
    bool isDead ();

    void handleUserInput (Game* game, CLIENT_INPUT userInput);
};

#endif