#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include "tad/city.h"

#include "tad/matrix.h"
#include "tad/tsp.h"


/* Renvoie une tournée aléatoire et   on calcule  longueur  en utilisant la matrice */
Results* randomWalk(Matrix* m);

#endif
