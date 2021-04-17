#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "GridComponent.h"

class Obstacle : public GridComponent {

public:
    Obstacle(); // Constructor for Obstacle
    Obstacle(GridPosition position); // Constructor for Obstacle
};

#endif