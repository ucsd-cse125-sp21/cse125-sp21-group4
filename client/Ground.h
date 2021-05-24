#ifndef _GROUND_H_
#define _GROUND_H_

#include <fstream>
#include <iostream>
#include "Core.h"
#include "SpatialHashTable.h"
#include "GroundTile.h"

// how many tiles would one side of the ground have as a square
#define GROUND_WIDTH_IN_TILES 600
#define TILE_SIZE 6.0

class Ground {
private:
	std::vector<glm::vec3> normal;
	std::vector<glm::vec3> points;
	std::vector<glm::ivec3> triangles;
	std::vector<glm::vec2> textCoord;

	glm::mat4* view;
	glm::mat4* projection;
	GLuint shader;

	GLuint textId;
	bool hasTexture;

	glm::vec3 eyep;

public:
	Ground(string fileName, glm::mat4* proj, glm::mat4* view, GLuint shader, char* textFile, 
		SpatialHashTable* table, float tileScale);
	void update();
	bool loadTexture(char* tPath);
};

#endif