#ifndef _EVOLVE_
#define _EVOLVE_

#include "Objective.h"


class Evolve : public Objective {
    private:
        float evo_amount; // amount to evolve

    public:
        Evolve(GridPosition position, Restriction r = R_MONSTER);

        float getEvoAmount();
};

#endif