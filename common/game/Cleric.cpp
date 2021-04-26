#include "Cleric.h"
#include "Game.h"

Cleric::Cleric() { 
    setType(CLERIC); // Fighter type game component
    setHp(CLERIC_MAX_HP); // init full health
    setAttackDamage(CLERIC_ATTACK_DAMAGE);
}

Cleric::Cleric(PlayerPosition position) : GamePlayer(position) {
    setType(CLERIC); // Fighter type game component
    setHp(CLERIC_MAX_HP); // init full health
    setAttackDamage(CLERIC_ATTACK_DAMAGE);
}

// overide GamePlayer's attack
void Cleric::attack(Game* game) {

}
