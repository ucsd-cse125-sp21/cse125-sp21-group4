#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "GameComponent.h"

class Obstacle : public GameComponent {

public:
    Obstacle(); // Constructor for Obstacle
    Obstacle(ComponentPosition position); // Constructor for Obstacle
};

#endif