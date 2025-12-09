#ifndef GA_H
#define GA_H

#include "../tad/matrix.h"
#include "../tad/tsp.h"

// Fonction principale à appeler depuis le main
Results* geneticAlgorithm(Matrix* m, int pSize, int maxGen, double mutRate, int crossCount, bool useDPX);

#endif