#include "Cell.h"

Cell::Cell(int iD) {
	next = NULL;
	items = vector<Object*>(0);
	id = iD;
}