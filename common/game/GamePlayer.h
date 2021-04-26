#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_
#include "GameConstants.h"
#include "Game.h"
#include <cmath>

// A GamePlayer's Bouding Box looks like
//          width
//     --------------
//     |            |
//     |            |
//     |    (x,y)   | height
//     |            |
//     |            |
//     --------------

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

    int id; // Used for determining playerID

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
    static bool samePosition (PlayerPosition p1, PlayerPosition p2);

    void move (Game* game, Direction direction, float distance); 
    void attack (Game* game, float distance);
    void hpDecrement (int amount);
    void interact (Game* game);
    bool isWithinObjective(Objective * objective);
    bool canInteractWithObjective(Objective * objective);
    bool isDead ();

    void handleUserInput (Game* game, CLIENT_INPUT userInput);

    void setID(int newID); // used to know what the id of the player is
    int getID();
};

#endif