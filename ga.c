#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tad/city.h"
#include "tad/matrix.h"
#include "tad/tsp.h"
#include "randomwalk.h"
#include "ga.h"

void mutate(int *path, int n) {
    int i = rand() % n;
    int j = rand() % n;
    int temp = path[i];
    path[i] = path[j];
    path[j] = temp;
}


static int contains(int *tab, int val, int start, int end) {
    for (int i = start; i <= end; i++) {
        if (tab[i] == val)
            return 1;
    }
    return 0;
}

int* crossover(int *parent1, int *parent2, int n) {
    int *child = malloc(n * sizeof(int));
    
    // On choisit une partie aléatoire du parent 1
    int start = rand() % n;
    int end = rand() % n;
    if (start > end) {
         int temp = start; 
         start = end; 
         end = temp; }

    // Copier cette partie dans l'enfant.
    for (int i = start; i <= end; i++) {
        child[i] = parent1[i];
    }

    // On remplit le reste avec les villes du deuxieme parent
    int current_p2 = 0;
    for (int i = 0; i < n; i++) {
        // Saut en cas ou on se situe dans la zone remplie par le 1er parent
        if (i >= start && i <= end)
            continue;
        // Trouver la prochaine ville valide dans le deuxieme
        while (contains(child, parent2[current_p2], start, end)) {
            current_p2++;
        }
        child[i] = parent2[current_p2];
        current_p2++;
    }
    return child;
}

Results* geneticAlgorithm(Matrix* m, int pSize, int maxGen, double mutRate, int crossCount){
    if (!m)
        return NULL;
    int dim = m->dimension;
    srand(time(NULL));
    Results* population[pSize];
    
    for (int i = 0; i < pSize; i++) {
        // On utilise votre randomWalk existant pour initialiser !
        population[i] = randomWalk(m); //
    }

    // Boucle Principale (Générations)
    for (int gen = 0; gen < maxGen; gen++){
        
        // Boucle de reproduction
        for (int k = 0; k < crossCount; k++) {
            
            // a. Sélectionner au hasard deux individus
            int idx1 = rand() % pSize;
            int idx2 = rand() % pSize;
            
            // b. Faire un croisement pour obtenir une fille
            int* childPath = crossover(population[idx1]->bestPath, population[idx2]->bestPath, dim);
            
            // c. Avec une probabilité p, faire muter la fille
            double r = (double)rand() / RAND_MAX;
            if (r < mutRate) {
                mutate(childPath, dim);
            }

            // Calculer la distance de la fille
            double childDist = totalPathDistance2(m, childPath, dim);

            // d. Remplacer un individu (ici : le moins performant)
            // On cherche le pire de la population
            int worstIdx = 0;
            double maxDist = -1;
            for(int i=0; i<pSize; i++){
                if(population[i]->bestDistance > maxDist){
                    maxDist = population[i]->bestDistance;
                    worstIdx = i;
                }
            }

            // Si l'enfant est meilleur que le pire, on remplace
            if (childDist < maxDist) {
                free(population[worstIdx]->bestPath);
                free(population[worstIdx]); // Libérer l'ancien

                // Créer le nouveau Result struct
                Results* newIndiv = malloc(sizeof(Results));
                newIndiv->dimension = dim;
                newIndiv->bestPath = childPath;
                newIndiv->bestDistance = childDist;
                
                population[worstIdx] = newIndiv;
            } else {
                // Sinon on jette l'enfant
                free(childPath);
            }
        }
    }

    // Fin : Trouver le meilleur de la population finale
    int bestIdx = 0;
    double minDist = population[0]->bestDistance; // Init avec le premier
    
    for (int i = 1; i < pSize; i++) {
        if (population[i]->bestDistance < minDist) {
            minDist = population[i]->bestDistance;
            bestIdx = i;
        }
    }

    // On doit retourner une COPIE du meilleur (car on va free la population)
    Results* finalResult = malloc(sizeof(Results));
    finalResult->dimension = dim;
    finalResult->bestDistance = population[bestIdx]->bestDistance;
    finalResult->bestPath = malloc(dim * sizeof(int));
    memcpy(finalResult->bestPath, population[bestIdx]->bestPath, dim * sizeof(int));

    // Nettoyage de la population
    for (int i = 0; i < pSize; i++) {
        free(population[i]->bestPath);
        free(population[i]);
    }

    return finalResult;
}