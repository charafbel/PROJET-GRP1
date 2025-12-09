#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <signal.h>
#include "../tad/matrix.h"
#include "nearestneighbor.h"
#include <float.h>


/**
 * @brief Resolution du TSP par l'heuristique du Plus Proche Voisin.
 * @param m Pointeur vers la matrice des distances.
 * @param k Indice de la ville de départ.
 * @return Results* Structure contenant le chemin trouvé et sa distance.
 * * @complexity O(N²).
 */
Results* nearestNeighbour(Matrix* m,int k){
    int dim = m->dimension;
    bool * visited = malloc(dim * sizeof(bool));
    for (int i = 0; i < dim; i++) {
        visited[i] = false;
    }
    Results* results = malloc(sizeof(Results));
    results->dimension = dim;
    results->bestPath = malloc(dim * sizeof(int));
    results->bestDistance = 0;
    int currentCity = k;
    visited[currentCity] = true;
    results->bestPath[0] = currentCity;

    for (int i = 1; i < dim; i++)
    {
        double dist_min = __DBL_MAX__;
        int nextCity;
        for (int j = 0; j < dim; j++)
        {
            if (!visited[j])
            {
                double dist = getDistance(m, currentCity, j);
                if (dist < dist_min)
                {
                    dist_min = dist;
                    nextCity = j;
                }
            }
        }
        results->bestPath[i] = nextCity;
        results->bestDistance += dist_min;
        visited[nextCity] = true;
        currentCity = nextCity;
    }
    results->bestDistance += getDistance(m, currentCity, k);
    free(visited);
    return results;
}


