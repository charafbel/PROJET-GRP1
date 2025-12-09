#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <float.h>
#include "../tad/city.h"
#include "../tad/matrix.h"
#include "../tad/tsp.h"
#include "../heuristiques/randomwalk.h"
#include "ga.h"

// --- Fonctions Utilitaires ---

void mutate(int *path, int n) {
    int i = rand() % n;
    int j = rand() % n;
    int temp = path[i];
    path[i] = path[j];
    path[j] = temp;
}

static int contains(int *tab, int val, int start, int end) {
    for (int i = start; i <= end; i++) {
        if (tab[i] == val) return 1;
    }
    return 0;
}

// Vérifie si l'arête (u, v) existe dans le tour (sens indifférent)
bool isEdgeInTour(int u, int v, int *tour, int n) {
    for (int i = 0; i < n; i++) {
        int current = tour[i];
        int next = tour[(i + 1) % n];
        if ((current == u && next == v) || (current == v && next == u)) {
            return true;
        }
    }
    return false;
}

// 2-opt local appliqué sur un tableau d'entiers (requis pour gadpx)
void applyTwoOpt(int *path, int n, Matrix *m) {
    bool amelioration = true;
    while (amelioration) {
        amelioration = false;
        for (int i = 0; i < n - 2; i++) {
            for (int j = i + 2; j < n; j++) {
                // Pour le dernier arc revenant au début
                if (i == 0 && j == n - 1) continue;

                int u = path[i];
                int v = path[i+1];
                int x = path[j];
                int y = path[(j+1) % n];

                double d_actuelle = getDistance(m, u, v) + getDistance(m, x, y);
                double d_nouvelle = getDistance(m, u, x) + getDistance(m, v, y);

                if (d_nouvelle < d_actuelle) {
                    // On inverse le segment entre i+1 et j
                    int left = i + 1;
                    int right = j;
                    while (left < right) {
                        int tmp = path[left];
                        path[left] = path[right];
                        path[right] = tmp;
                        left++;
                        right--;
                    }
                    amelioration = true;
                }
            }
        }
    }
}

// --- Croisements ---

/**
 * @brief Effectue un croisement standard.
 * * @param parent1 Tableau du premier parent.
 * @param parent2 Tableau du second parent.
 * @param n Dimension.
 * @return Numero de l'enfant.
 */
int* crossover(int *parent1, int *parent2, int n) {
    int *child = malloc(n * sizeof(int));

    int start = rand() % n;
    int end = rand() % n;
    if (start > end) { int temp = start; start = end; end = temp; }

    for (int i = start; i <= end; i++) {
        child[i] = parent1[i];
    }

    int idxP2 = 0;
    for (int i = 0; i < n; i++) {
        if (i >= start && i <= end) continue;
        while (contains(child, parent2[idxP2], start, end)) {
            idxP2++;
        }
        child[i] = parent2[idxP2];
        idxP2++;
    }
    return child;
}

// Croisement DPX (Distance Preserving Crossover)

/**
 * @brief Croisement DPX.
 * * @param parent1 Tableau du premier parent.
 * @param parent2 Tableau du second parent.
 * @param n Dimension.
 * @param m Matrice des distances.
 * @return int* Nouvelle tournée.
 * * @complexity O(N²).
 */
int* crossoverDPX(int *parent1, int *parent2, int n, Matrix *m) {
    int *child = malloc(n * sizeof(int));
    bool *visited = calloc(n, sizeof(bool));

    // Tableau pour stocker les arêtes communes forcées (next_node[u] = v)
    int *next_node = malloc(n * sizeof(int));
    for(int i=0; i<n; i++) next_node[i] = -1;

    // 1. Identification des arêtes communes
    for (int i = 0; i < n; i++) {
        int u = parent1[i];
        int v = parent1[(i + 1) % n];

        // Si l'arête (u,v) est présente dans le parent 2 aussi
        if (isEdgeInTour(u, v, parent2, n)) {
            next_node[u] = v;
        }
    }

    // 2. Reconstruction
    int current = parent1[0];

    for (int step = 0; step < n; step++) {
        child[step] = current;
        visited[current] = true;

        if (step == n - 1) break;

        // Si une arête commune existe vers un sommet non visité, on la prend
        if (next_node[current] != -1 && !visited[next_node[current]]) {
            current = next_node[current];
        } else {
            // Sinon, Plus Proche Voisin (Nearest Neighbor) parmi les non visités
            double minDist = DBL_MAX;
            int bestNext = -1;

            for (int candidate = 0; candidate < n; candidate++) {
                if (!visited[candidate]) {
                    double d = getDistance(m, current, candidate);
                    if (d < minDist) {
                        minDist = d;
                        bestNext = candidate;
                    }
                }
            }
            // Sécurité si on ne trouve personne (ne devrait pas arriver sur un graphe complet)
            if (bestNext == -1) {
                for(int k=0; k<n; k++) if(!visited[k]) { bestNext = k; break; }
            }
            current = bestNext;
        }
    }

    free(next_node);
    free(visited);
    return child;
}

// --- Algorithme Principal ---

/**
 * @brief Algorithme Génétique Principal.
 * * @param m Matrice des distances.
 * @param pSize Taille de la population.
 * @param maxGen Nombre maximum de générations.
 * @param mutRate Taux de mutation.
 * @param crossCount Nombre de croisements par génération.
 * @param useDPX Booléen pour activer le mode DPX optimisé.
 * @return Results* La meilleure solution trouvée.
 */
Results* geneticAlgorithm(Matrix* m, int pSize, int maxGen, double mutRate, int crossCount, bool useDPX){
    if (!m) return NULL;
    int dim = m->dimension;
    srand(time(NULL));

    Results* population[pSize];

    // Population initiale aléatoire
    for (int i = 0; i < pSize; i++) {
        population[i] = randomWalk(m);
    }

    for (int gen = 0; gen < maxGen; gen++){
        for (int k = 0; k < crossCount; k++) {

            int idx1 = rand() % pSize;
            int idx2 = rand() % pSize;

            int* childPath;

            if (useDPX) {
                // CAS GADPX : DPX + 2-opt, sans mutation aléatoire
                childPath = crossoverDPX(population[idx1]->bestPath, population[idx2]->bestPath, dim, m);
                applyTwoOpt(childPath, dim, m);
            } else {
                // CAS GA STANDARD : Crossover + Mutation
                childPath = crossover(population[idx1]->bestPath, population[idx2]->bestPath, dim);
                double r = (double)rand() / RAND_MAX;
                if (r < mutRate) {
                    mutate(childPath, dim);
                }
            }

            double childDist = totalPathDistance2(m, childPath, dim);

            // Remplacement du pire (Steady State)
            int worstIdx = 0;
            double maxDist = -1;
            for(int i=0; i<pSize; i++){
                if(population[i]->bestDistance > maxDist){
                    maxDist = population[i]->bestDistance;
                    worstIdx = i;
                }
            }

            if (childDist < maxDist) {
                free(population[worstIdx]->bestPath);
                free(population[worstIdx]);

                Results* newIndiv = malloc(sizeof(Results));
                newIndiv->dimension = dim;
                newIndiv->bestPath = childPath;
                newIndiv->bestDistance = childDist;

                population[worstIdx] = newIndiv;
            } else {
                free(childPath);
            }
        }
    }

    // Sélection du meilleur final
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