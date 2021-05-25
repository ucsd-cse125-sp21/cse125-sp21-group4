#ifndef _CELL_H_
#define _CELL_H_

#include "core.h"
#include "EnvElement.h"
#include "Object.h"
#include <vector>

class Cell {
public:
	int id;
	Cell(int);
	Cell* next;
	vector<Object*> items;
};

#endif