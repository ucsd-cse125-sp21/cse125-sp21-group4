#include "Mage.h"
#include "Game.h"

Mage::Mage() { 
    setType(MAGE); // Fighter type game component
    setHp(MAGE_MAX_HP); // init full health
    setAttackHarm(MAGE_ATTACK_HARM);
}

Mage::Mage(PlayerPosition position) : GamePlayer(position) {
    setType(MAGE); // Fighter type game component
    setHp(MAGE_MAX_HP); // init full health
    setAttackHarm(MAGE_ATTACK_HARM);
}

// overide GamePlayer's attack
void Mage::attack(Game* game) {

}
