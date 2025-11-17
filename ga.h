#ifndef GA_H
#define GA_H

#include "tad/matrix.h"
#include "tad/tsp.h"

// Paramètres de l'algorithme (comme suggéré dans l'image)
#define POP_SIZE 50          // Taille de la population
#define MAX_GENERATIONS 1000 // Critère d'arrêt
#define MUTATION_RATE 0.2    // Probabilité de mutation (20%)
#define CROSSOVER_COUNT 20   // Nombre de croisements par génération

// Fonction principale à appeler depuis le main
Results* geneticAlgorithm(Matrix* m);

#endif