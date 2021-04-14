#include "GameComponent.h"

GameComponent::GameComponent() : type(SPACE){}

GameComponent::GameComponent (GameComponentType type) : type(type) {}

GameComponentType GameComponent::getType() { return type; }

void GameComponent::setType(GameComponentType newType) { type = newType; }

ComponentPosition GameComponent::getPosition() { return  position; }

void GameComponent::setPosition (int x, int y) { 
    position.x = x;
    position.y = y;
}

bool GameComponent::isSpace () { return type == SPACE; }

bool GameComponent::isMonster () { return type == MONSTER; }

bool GameComponent::isFighter () { return type == FIGHTER; }

bool GameComponent::isObstacle () { return type == OBSTACLE; }


