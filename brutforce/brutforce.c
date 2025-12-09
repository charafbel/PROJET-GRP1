#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "../tad/matrix.h"
#include "../tad/tsp.h"


void swapArrVal(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * @brief Génère la permutation suivante
 * @param arr Le tableau représentant la tournée.
 * @return true si une nouvelle permutation a été générée, false sinon
*/
bool nextPermutation(int *arr, int n){
    int i = n-2; // On part de l'avant-dernier element
    // trouver l'indice le plus haut tel que arr[i] < arr[i + 1].
    while (i >= 0 && arr[i] >= arr[i + 1])
        i--;
    if (i < 0)
        return false;
    // trouver le plus petit élément à droite de i qui soit > à arr[i]
    int j = n-1;
    while (arr[j] <= arr[i])
        j--;
    // echange
    swapArrVal(&arr[i], &arr[j]);
    // inversion de la fin du tableau.
    int k = i + 1;
    int l = n - 1;
    while (k < l){
        swapArrVal(&arr[k], &arr[l]);
        k++;
        l--;
    }
    return true;
}


/**
 * @brief Calcule la distance totale d'une tournée.
 * * @param m Pointeur vers la matrice des distances.
 * @param path Tableau représentant l'ordre de visite des villes.
 * @param n Nombre de villes.
 * @return La distance totale.
 * * @complexity O(N)
 */
int totalPathDistance(Matrix *m, const int *path, int n){
    int sum = 0;
    for (int i = 0; i < n - 1; i++){
        sum += getDistance(m, path[i], path[i + 1]);
    }
    sum += getDistance(m, path[n - 1], path[0]);
    return sum;
}


/**
 * @brief Implementation de la resolution du TSP par force brute
 * * @param m Pointeur vers la matrice des distances.
 * @return Results* Structure contenant le meilleur chemin et la distance de celui ci.
 * * @complexity O(N * N!) - C'est une complexité factorielle. 
 */
Results* brutForce(Matrix *m){
    int dim = m->dimension;
    int *idx = malloc(dim * sizeof(int));
    int *best = malloc(dim * sizeof(int));

    if (!idx || !best) {
        fprintf(stderr, "Error malloc (brutforce)\n");
        exit(EXIT_FAILURE);
    }

    // Tableau d'indices
    for (int i = 0; i < dim; i++){
        idx[i] = i;
    }

    int bestDist = INT_MAX; // On initialise la meilleure distance à une valeur ultra elevée et non depassable par le type int

    do { /* Test des permutations et allocation de la meilleure distance */
        int d = totalPathDistance(m, idx, dim);
        if (d < bestDist) {
            bestDist = d;
            memcpy(best, idx, dim * sizeof(int));
        }
    } while (nextPermutation(idx, dim));

    // Allocation des resultats dans la structure results
    Results* results = malloc(sizeof(Results));
    results->dimension = dim;
    results->bestDistance = bestDist;
    results->bestPath = malloc(dim * sizeof(int));
    memcpy(results->bestPath, best, dim * sizeof(int));

    // Renvoie et Nettoyage memoire
    free(idx);
    free(best);
    return results;
}