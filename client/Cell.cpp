#include "Cell.h"

Cell::Cell(int iD) {
	next = NULL;
	items = vector<EnvElement*>(0);
	id = iD;
}