#ifndef _EVOLVE_
#define _EVOLVE_

#include "Objective.h"

class Evolve : public Objective {
    private:
        int evo_amount; // amount to evolve

    public:
        Evolve(GridPosition position, Restriction r);

        int getEvoAmount();
};

#endif