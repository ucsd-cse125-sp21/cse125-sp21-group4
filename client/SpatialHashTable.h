#ifndef _SPATIALHASHTABLE_H_
#define _SPATIALHASHTABLE_H_

#include "core.h"
#include "EnvElement.h"
#include "Cell.h"
#include <vector>

class SpatialHashTable {
private:
	vector<Cell*> cellList;
	int listSize;
	float h;
	int count;
public:
	SpatialHashTable();
	SpatialHashTable(int size, float dis);
	Cell* getCell(glm::vec3);
	void insert(EnvElement*);
	void remove(EnvElement*);
	float getDistance();
};

#endif