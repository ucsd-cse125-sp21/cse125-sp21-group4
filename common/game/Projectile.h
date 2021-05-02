
#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "GameConstants.h"

class Projectile {
public:
    ProjectilePosition origin;
    ProjectilePosition currentPosition;
    float maxDistance;
    int ownerID;
    ProjectileType type;
    float speed; // traveling distance per server tick
    Direction direction; // the direction it flies to
    float damage;
};


#endif