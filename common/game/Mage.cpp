#include "Mage.h"
#include "Game.h"

Mage::Mage() { 
    setType(MAGE); // Fighter type game component
    setHp(MAGE_MAX_HP); // init full health
    setAttackDamage(MAGE_ATTACK_DAMAGE);
}

Mage::Mage(PlayerPosition position) : GamePlayer(position) {
    setType(MAGE); // Fighter type game component
    setHp(MAGE_MAX_HP); // init full health
    setAttackDamage(MAGE_ATTACK_DAMAGE);
}

// overide GamePlayer's attack
void Mage::attack(Game* game) {

}
