#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Core.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Object{
public:
	virtual void draw(glm::mat4 c) = 0;
	virtual void update() = 0;
	virtual void updateView(glm::mat4, glm::vec3) = 0;
};

#endif