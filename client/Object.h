#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Core.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/*
	Object is now supported by the spatial hash table. Any subclass now can be used
	in spatial hash table for proximity rendering.
*/

class Object{
public:
	glm::vec3 pos; // pos now moved to Object class to have support in spatial hash table
	virtual void draw(glm::mat4 c) = 0;
	virtual void update() = 0;
	virtual void updateView(glm::mat4, glm::vec3) = 0;
	virtual void drawIfNotObstructing(glm::vec3 clientPos, glm::mat4 c = glm::mat4(1)) = 0;
};

#endif