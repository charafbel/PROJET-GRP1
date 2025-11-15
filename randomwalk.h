#ifndef RANDOMWALK_H
#define RANDOMWALK_H
#include <stdlib.h>
#include <string.h>
#include "tad/matrix.h"



/* Renvoie une tournée aléatoire et on calcule longueur en utilisant la matrice */
Results* randomWalk(Matrix* m);
Results* twoOptrw(Matrix* mat, Results* res);

#endif
