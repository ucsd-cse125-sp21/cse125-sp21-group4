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

// used for bfs search around the player
#include <queue>
#include <vector>


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
    int maxHp;
    int attackDamage; // harmness made per attack
    Direction faceDirection; // the direction the player is facing
    float speed; // current move speed
    float acceleration; // acceleration relative to server tick
    float maxSpeed; // the maximum speed player can do
    std::chrono::steady_clock::time_point prevTime;
    std::chrono::steady_clock::time_point deathTime;

    int id; // Used for determining playerID
    int specID; // ID of the player this player is spectating

    long long getTimeDiff();

public:
    GamePlayer(); // Constructor for GamePlayer
    GamePlayer(PlayerPosition position); // Constructor for GameComponent

    PlayerType getType();
    void setType(PlayerType newType);
    const std::string getTypeToString();
    PlayerPosition getPosition();
    void setPosition(PlayerPosition newPosition);
    int getMaxHp();
    int getHp ();
    void setHp (int newHp);
    int getAttackDamage ();
    void setAttackDamage (int newAttackDamage);
    Direction getFaceDirection (); // faceDirection getter
    void setFaceDirection (Direction newDirection); // faceDirection setter
    float getSpeed ();
    void setSpeed (float newSpeed);
    float getAcceleration ();
    void setAcceleration (float newAcceleration);
    float getMaxSpeed ();
    void setMaxSpeed (float newMaxSpeed);
    void slowDown (float amount);
    void speedUp (float amount);
    void speedChange(float amount);

    float getUpperLeftCoordinateX (PlayerPosition position, bool isPlayer);
    float getUpperLeftCoordinateY (PlayerPosition position, bool isPlayer);
    float getBottomRightCoordinateX (PlayerPosition position, bool isPlayer);
    float getBottomRightCoordinateY (PlayerPosition position, bool isPlayer);

    bool isCollidingPlayer(Game* game, PlayerPosition currentPosition);
    bool isCollidingObstacle(Game* game, PlayerPosition currentPosition);
    bool canMoveTo(Game* game, PlayerPosition position);
    bool canAttack (GamePlayer* player);
    static bool samePosition (PlayerPosition p1, PlayerPosition p2);

    void move (Game* game, Direction direction); 
    virtual void attack (Game* game, float angle);
    virtual void uniqueAttack (Game* game, float angle);
    virtual void hpDecrement (int damage, bool fromSystem);
    void hpIncrement (int heal);
    void interactHeal (Game* game, Heal* heal);
    void interactArmor (Game* game, Armor* armor);
    virtual void interact (Game* game);
    void revive(Game* game);
    bool isWithinObjective(Objective * objective);
    bool canInteractWithObjective(Objective * objective);
    bool isDead ();
    
    
    bool isEnemy (GamePlayer* otherPlayer);

    void handleUserInput (Game* game, GAME_INPUT userInput);

    void setID(int newID); // used to know what the id of the player is
    int getID();

    void spectate(Game* game, UPDATE_TYPE update);

    PlayerPosition bfsSearchNonCollidingPosition(Game*);
};

#endif
