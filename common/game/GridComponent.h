#ifndef _GRIDCOMPONENT_H_
#define _GRIDCOMPONENT_H_
#include "GameConstants.h"

struct GridPosition {
    int x;
    int y;
}; 
typedef GridPosition GridPosition;

enum GridComponentType {
    SPACE, // 0
    OBSTACLE, // 1
    ROCK, // 2
    OBJECTIVE, // 3
};


class GridComponent {
private:
    GridComponentType type;
    GridPosition position;

public:
    GridComponent(); // Default Constructor
    GridComponent(GridComponentType type); // Constructor for GridComponent
    
    GridComponentType getType(); // type getter
    void setType(GridComponentType newType); // type getter
    
    GridPosition getPosition(); // position getter
    void setPosition(int x, int y);  // position setter

    bool isSpace();
    bool isObstacle ();
};


typedef GridComponent* Grids [MAP_WIDTH/GRID_WIDTH][MAP_HEIGHT/GRID_HEIGHT];

#endif