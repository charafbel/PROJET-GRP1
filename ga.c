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
    
    //On choisit une partie aléatoire du parent 1
    int start = rand() % n;
    int end = rand() % n;
    if (start > end) {
         int temp = start; 
         start = end; 
         end = temp; }

    //Copier cette partie dans l'enfant.
    for (int i = start; i <= end; i++) {
        child[i] = parent1[i];
    }

    // On remplit le reste avec les villes du deuxieme parent
    int partie_parent2 = 0;
    for (int i = 0; i < n; i++) {
        // n'ecrase pas la partie du parent 1 
        if (i >= start && i <= end)
            continue;
        //trouver les villes valide dans le parent 2 
        while (contains(child, parent2[partie_parent2], start, end)) {
            partie_parent2++;
        }
        child[i] = parent2[partie_parent2];
        partie_parent2++;
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
        population[i] = randomWalk(m); // créer une population initiale de N individus 
    }

    //le nombre fixé de générations ou la stabilité est atteint;
    for (int gen = 0; gen < maxGen; gen++){

        for (int k = 0; k < crossCount; k++) {
            //sélectionner au hasard deux individus
            int idx1 = rand() % pSize;
            int idx2 = rand() % pSize;
            //faire des croisement eux qui donnent une tournée fille;
            int* childPath = crossover(population[idx1]->bestPath, population[idx2]->bestPath, dim);
            
            // avec une probabilité p faire muter la fille;
            double r = (double)rand() / RAND_MAX;
            if (r < mutRate) {
                mutate(childPath, dim);
            }

            //calcul de la distance totale de la tournée fille
            double childDist = totalPathDistance2(m, childPath, dim);

            //remplacer un individu de la population par la fille (le moins performant)
            int worstIdx = 0;
            double maxDist = -1;
            for(int i=0; i<pSize; i++){
                if(population[i]->bestDistance > maxDist){
                    maxDist = population[i]->bestDistance;
                    worstIdx = i;
                }
            }

            //remplacement du pire par l'enfant
            if (childDist < maxDist) {
                free(population[worstIdx]->bestPath);
                free(population[worstIdx]);

                //struct results utilisé pour nos individus
                Results* newIndiv = malloc(sizeof(Results));
                newIndiv->dimension = dim;
                newIndiv->bestPath = childPath;
                newIndiv->bestDistance = childDist;
                
                population[worstIdx] = newIndiv;
            } else {
                // on garde l'ancien plus besoin d'allouer de la memoire pour le child
                free(childPath);
            }
        }
    }

    //retourner le meilleur individu de la population
    int bestIdx = 0;
    double minDist = population[0]->bestDistance; 
    for (int i = 1; i < pSize; i++) {
        if (population[i]->bestDistance < minDist) {
            minDist = population[i]->bestDistance;
            bestIdx = i;
        }
    }
    Results* finalResult = malloc(sizeof(Results));
    finalResult->dimension = dim;
    finalResult->bestDistance = population[bestIdx]->bestDistance;
    finalResult->bestPath = malloc(dim * sizeof(int));
    memcpy(finalResult->bestPath, population[bestIdx]->bestPath, dim * sizeof(int));

    for (int i = 0; i < pSize; i++) {
        free(population[i]->bestPath);
        free(population[i]);
    }

    return finalResult;
}