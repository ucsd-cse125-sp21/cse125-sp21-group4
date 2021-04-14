#ifndef _GAMECOMPONENT_H_
#define _GAMECOMPONENT_H_
#include "GameConstants.h"

struct ComponentPosition {
    int x;
    int y;
}; 
typedef ComponentPosition ComponentPosition;


class GameComponent {
private:
    GameComponentType type;
    ComponentPosition position;

public:
    GameComponent(); // Default Constructor
    GameComponent(GameComponentType type); // Constructor for GameComponent
    GameComponentType getType(); // type getter
    void setType(GameComponentType newType); // type getter
    ComponentPosition getPosition(); // position getter
    void setPosition(int x, int y);  // position setter

    bool isSpace();
    bool isMonster();
    bool isFighter ();
    bool isObstacle ();
};


typedef GameComponent* Grids [MAP_WIDTH][MAP_HEIGHT];

#endif