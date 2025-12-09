#include <stdlib.h>
#include <time.h>
#include "../tad/matrix.h"
#include "../tad/tsp.h"
#include "randomwalk.h"
#include <stdbool.h>
#include <float.h>

// Fonctions utilitaires :
static void swapArrVal(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void reverse_segment(int* path, int start, int end) {
    while (start < end) {
        swapArrVal(&path[start], &path[end]);
        start++;
        end--;
    }
}

/* Determine longueur cycle fermé qui est défini par chemin[0/n-1]  */
double totalPathDistance2(Matrix *m, const int *chemin, int n) {
    double somme = 0;
    for (int i = 0; i < n - 1; i++) {
        somme += getDistance(m, chemin[i], chemin[i + 1]);
    }
    somme += getDistance(m, chemin[n - 1], chemin[0]); /* retour au départ */
    return somme;
}

static void shuffle(int *arr, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

Results *randomWalk(Matrix *m) {
    if (!m || m->dimension <= 0) return NULL;

    /*  RNG une seule fois par processus */
    static int x = 0;
    if (!x)
        srand((unsigned int) time(NULL));
    x = 1;

    int n = m->dimension;

    /* construire une permutation 0..n-1 puis mélanger */
    int *perm = (int *) malloc(sizeof(int) * n);
    if (!perm) {
        perror("malloc randomWalk perm");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; ++i) perm[i] = i;
    shuffle(perm, n);

    /* distance de la tournée */
    double dist = totalPathDistance2(m, perm, n);

    /* empaqueter dans Results (mêmes conventions que le reste du projet) */
    Results *result = (Results *) malloc(sizeof(Results));
    if (!result) {
        perror("malloc mauvais randomWalk resultat");
        exit(EXIT_FAILURE);
    }
    result->bestDistance = dist;

    result->dimension = n;
    result->bestPath = (int *) malloc(sizeof(int) * n);
    if (!result->bestPath) {
        perror("malloc mauvais  randomWalk chemin");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; ++i) { result->bestPath[i] = perm[i]; }

    free(perm);
    return result;
}

Results* twoOptrw(Matrix* mat, Results* res){
    if (!mat || !res) {
        perror("twoOpt mat or res null");
        exit(EXIT_FAILURE);
    }
    if (res->dimension != mat->dimension) {
        perror("twoOpt dimension mismatch");
        exit(EXIT_FAILURE);
    }

    int n = res->dimension;
    int* path = res->bestPath;
    double dist = res->bestDistance;
    bool stop = true;
    
    while (stop){
        stop = false;
        for (int i = 0; i <= (n-3); i++){
            for (int j = i + 2; j < n; j++){
                int k = (j+1)%n;
                double old_dist = getDistance(mat, path[i], path[i+1])
                                + getDistance(mat, path[j], path[k]);
                
                double new_dist = getDistance(mat, path[i], path[j])
                                + getDistance(mat, path[i+1], path[k]);

                double diff = new_dist-old_dist;
                if (diff < 0){
                    reverse_segment(path, i + 1, j);
                    dist += diff;
                    stop = true;
                    break;
                }
            } 
            if (stop)
                break;
        }
    }
    res->bestDistance = dist;
    res->bestPath = path;
    return res;
}