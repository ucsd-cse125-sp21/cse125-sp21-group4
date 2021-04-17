#include "GridComponent.h"

GridComponent::GridComponent() : type(SPACE){}

GridComponent::GridComponent (GridComponentType type) : type(type) {}

GridComponentType GridComponent::getType() { return type; }

void GridComponent::setType(GridComponentType newType) { type = newType; }

GridPosition GridComponent::getPosition() { return  position; }

void GridComponent::setPosition (int x, int y) { 
    position.x = x;
    position.y = y;
}

bool GridComponent::isSpace () { return type == SPACE; }

bool GridComponent::isObstacle () { return type == OBSTACLE; }


