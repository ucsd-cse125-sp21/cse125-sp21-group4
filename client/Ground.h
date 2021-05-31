#ifndef _GROUND_H_
#define _GROUND_H_

#include <fstream>
#include <iostream>
#include "Core.h"
#include "SpatialHashTable.h"
#include "GroundTile.h"

// how many tiles would one side of the ground have as a square
#define GROUND_WIDTH 600
#define TILE_SIZE 6.0

#define DRY_GRASS_ID 0
#define CRACKED_TILE_ID 1

class Ground {
private:
	std::vector<glm::vec3> normal;
	std::vector<glm::vec3> points;
	std::vector<glm::ivec3> triangles;
	std::vector<glm::vec2> textCoord;

	vector<glm::vec3> grassTileTrans;
	vector<glm::vec3> roadTileTrans;

	GroundTile * roadTiles;
	GroundTile * grassTiles;

public:
	Ground(string fileName, glm::mat4* proj, glm::mat4* view, GLuint shader, char* grassTextFile,  
		   char* crackedTileTextFile, float tileScale);
	void update();
	void draw();
};

#endif