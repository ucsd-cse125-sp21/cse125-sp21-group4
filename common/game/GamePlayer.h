#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_
#include "GameConstants.h"
#include "Game.h"
#include <cmath>

// Include Objective types to implement interactions with objectives
#include "Armor.h"
#include "Beacon.h"
#include "Evolve.h"
#include "Healing.h"

// A GamePlayer's Bouding Box looks like
//          width
//     --------------
//     |            |
//     |            |
//     |    (x,y)   | height
//     |            |
//     |            |
//     --------------



class GamePlayer {
protected:
    PlayerType type;
    PlayerPosition position; // position struct for player
    int hp; // health for GamePlayer
    int maxHP; // max health for GamePlayer
    int attackDamage; // harmness made per attack
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
    int getAttackDamage ();
    void setAttackDamage (int newAttackDamage);
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
    virtual void attack (Game* game);
    void hpDecrement (int damage);
    void hpIncrement (int heal);
    void interactHeal (Game* game, Heal* heal);
    void interactArmor (Game* game, Armor* armor);
    virtual void interact (Game* game);
    bool isWithinObjective(Objective * objective);
    bool canInteractWithObjective(Objective * objective);
    bool isDead ();
    
    bool isEnemy (GamePlayer* otherPlayer);

    void handleUserInput (Game* game, CLIENT_INPUT userInput);

    void setID(int newID); // used to know what the id of the player is
    int getID();
};

#endif