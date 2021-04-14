#include "Fighter.h"
#include "Game.h"

Fighter::Fighter() { 
    setType(FIGHTER); // Fighter type game component
    setHp(FIGHTER_MAX_HP); // init full health
    setAttackHarm(FIGHTER_ATTACK_HARM);
}

Fighter::Fighter(ComponentPosition position) : GamePlayer(position) {
    setType(FIGHTER); // Fighter type game component
    setHp(FIGHTER_MAX_HP); // init full health
    setAttackHarm(FIGHTER_ATTACK_HARM);
}

// overide GamePlayer's attack
void Fighter::attack(Game* game) {
    int x = getPosition().x;
    int y = getPosition().y;

    // if player's position is invalid, return directly
    if (invalidPlayerPosition()) return;

    GameComponent* targetComponent = game->getGameComponentInDirection(x, y, getFaceDirection());

    if (targetComponent->isMonster()) {
        ((GamePlayer*) targetComponent)->hpDecrement(getAttackHarm());
    }
}
