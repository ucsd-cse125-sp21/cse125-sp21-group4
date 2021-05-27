
#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "GameConstants.h"

class Projectile {
public:
    ProjectilePosition origin;
    ProjectilePosition currentPosition;
    float deltaX; // the distance this proj will move each server tick
    float deltaY; // the distance this proj will move each server tick
    float maxDistance;
    int ownerID;
    ProjectileType type;
    float damage;
};


#endif