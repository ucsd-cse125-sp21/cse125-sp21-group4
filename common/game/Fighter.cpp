#include "Fighter.h"
#include "Game.h"

Fighter::Fighter() { 
    setType(FIGHTER); // Fighter type game component
    setHp(FIGHTER_MAX_HP); // init full health
    setAttackHarm(FIGHTER_ATTACK_HARM);
}

Fighter::Fighter(PlayerPosition position) : GamePlayer(position) {
    setType(FIGHTER); // Fighter type game component
    setHp(FIGHTER_MAX_HP); // init full health
    setAttackHarm(FIGHTER_ATTACK_HARM);
}

// overide GamePlayer's attack
void Fighter::attack(Game* game) {

}
