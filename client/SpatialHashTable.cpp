#include "SpatialHashTable.h"

SpatialHashTable::SpatialHashTable() {
	listSize = 2000;
	h = 0.01f;
	cellList = vector<Cell*>(listSize, NULL);
	count = 0;
}

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

void SpatialHashTable::insert(EnvElement* p) {
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

void SpatialHashTable::remove(EnvElement* p) {
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