#include "Monster.h"
#include "Game.h"


Monster::Monster() { 
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    setAttackHarm(MONSTER_ATTACK_HARM);
}

Monster::Monster(ComponentPosition position) : GamePlayer(position){
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    setAttackHarm(MONSTER_ATTACK_HARM);
}

void Monster::attack(Game* game) {
    int x = getPosition().x;
    int y = getPosition().y;

    // if player's position is invalid, return directly
        if (invalidPlayerPosition()) {
        // do something
        return;
    }

    GameComponent* targetComponent = game->getGameComponentInDirection(x, y, faceDirection);

    // if the target grid contains the enemy (fighter), attack!
    if (targetComponent->isFighter()) {
        ((GamePlayer*) targetComponent)->hpDecrement(attackHarm);
    }
}



