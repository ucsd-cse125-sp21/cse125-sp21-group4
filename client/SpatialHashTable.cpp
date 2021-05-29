#include "SpatialHashTable.h"

SpatialHashTable::SpatialHashTable() {
	listSize = 2000;
	h = 0.01f;
	cellList = vector<Cell*>(listSize, NULL);
	count = 0;
}


/*
	size: how many cells are going to be used in this hash table; default is 2000.
	dis:  this is the width of a grid box in the 3d space; the bigger this value
		  is, the larger the amount of 3d space a grid box would cover. Generally
		  you want larger grid boxes if you want to commute farther objects, and
		  smaller grid boxe if you only want to consider near by objects. Default is
		  0.01f.
*/
SpatialHashTable::SpatialHashTable(int size, float dis) {
	listSize = 2000;
	if (size > listSize)
		listSize = size;
	h = 0.01f;
	h = dis;
	cellList = vector<Cell*>(listSize, NULL);
	count = 0;
}

Cell* SpatialHashTable::getCell(glm::vec3 p) {
	int iD = (int)(p.x / h) + (int)(p.y / h) + (int)(p.z / h);

	int index = ((((int)(p.x / h) * 73856093) ^
		((int)(p.y / h) * 19349663) ^
		((int)(p.z / h) * 83492791)) % (unsigned int)listSize);

	if (cellList[index] == NULL)
		return NULL;
	Cell* curr = cellList[index];
	while (curr != NULL) {
		if (curr->id == iD) {
			return curr;
		}
		curr = curr->next;
	}

	return NULL;
}

void SpatialHashTable::insert(Object* p) {
	int iD = (int)(p->pos.x / h) + (int)(p->pos.y / h) + (int)(p->pos.z / h);
	int index = ((((int)(p->pos.x / h) * 73856093) ^
		((int)(p->pos.y / h) * 19349663) ^
		((int)(p->pos.z / h) * 83492791)) % (unsigned int)listSize);

	Cell* cell = cellList[index];
	Cell* parent = NULL;

	// if cell has not been created, create a cell, and the insert
	if (cell == NULL) {
		cellList[index] = new Cell(iD);
		cellList[index]->items.push_back(p);
		return;
	}
	else {
		while (cell != NULL) {
			if (cell->id == iD) {
				cell->items.push_back(p);
				return;
			}
			parent = cell;
			cell = cell->next;
		}
		//if no existing cell is compatible
		parent->next = new Cell(iD);
		parent->next->items.push_back(p);
		return;
	}
}

void SpatialHashTable::remove(Object* p) {
	int iD = (int)(p->pos.x / h) + (int)(p->pos.y / h) + (int)(p->pos.z / h);
	int index = ((((int)(p->pos.x / h) * 73856093) ^
		((int)(p->pos.y / h) * 19349663) ^
		((int)(p->pos.z / h) * 83492791)) % (unsigned int)listSize);
	Cell* cell = cellList[index];

	if (cell == NULL)
		return;

	while (cell != NULL) {
		if (cell->id == iD) {
			int i;
			for (i = 0; i < cell->items.size(); i++) {
				//if (cell->items[i]->id == p->id) {
				if (cell->items[i] == p) {
					cell->items.erase(cell->items.begin() + i);
					return;
				}
			}
		}
		cell = cell->next;
	}
	return;
}

float SpatialHashTable::getDistance() {
	return this->h;
}