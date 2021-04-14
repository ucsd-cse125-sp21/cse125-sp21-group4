#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_
#include "GameConstants.h"
#include "GameComponent.h"
#include "Game.h"



class GamePlayer : public GameComponent {
protected:
    int hp; // health for GamePlayer
    int attackHarm; // harmness made per attack
    Direction faceDirection; // the direction the player is facing

public:
    GamePlayer(); // Constructor for GameComponent
    GamePlayer(ComponentPosition position); // Constructor for GameComponent

    int getHp ();
    void setHp (int newHp);
    int getAttackHarm ();
    void setAttackHarm (int newAttackHarm);
    Direction getFaceDirection (); // faceDirection getter
    void setFaceDirection (Direction newDirection); // faceDirection setter

    bool invalidPlayerPosition();
    void move (Game* game, Direction direction); 
    void attack (Game* game);
    void hpDecrement (int amount);
    bool isDead ();
};

#endif