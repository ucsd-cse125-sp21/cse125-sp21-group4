#include "Monster.h"
#include "Game.h"


Monster::Monster() { 
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
}

Monster::Monster(PlayerPosition position) : GamePlayer(position){
    setType(MONSTER); // Monster type game component
    setHp(MONSTER_MAX_HP); // init full health
    setAttackDamage(MONSTER_ATTACK_DAMAGE);
}

// Monster Spit attack goes here
void Monster::attack(Game* game) {

}



