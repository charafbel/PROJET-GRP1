#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <signal.h>
#include <string.h>

#include "tad/city.h"
#include "tad/matrix.h"
#include "tad/tsp.h"


void swatArrVal(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

bool nextPermutation(int *arr, int n) {
    int i = n-2;
    while (i >= 0 && arr[i] >= arr[i+1]){
        i--;
    }

    if (i < 0) return false;  // derniere permutation (problème memoire)
    int j = n-1;

    while (arr[j] <= arr[i]) {
        j--;
    }
    swatArrVal(&arr[i], &arr[j]); // Echange des deux valeurs

    //Inversion de la fin de tableau.
    int k = i+1;
    int l = n-1;
    while (k < j){
        swatArrVal(&arr[k], &arr[l]);
        k++;
        l--;
    }

    return true;
}

int totalPathDistance(Matrix *m, int *chemin, int n) {
    int sum = 0;
    for (int i = 0; i < n - 1; i++){
        sum += getDistance(m, chemin[i], chemin[i + 1]);
    }
    sum += getDistance(m, chemin[n - 1], chemin[0]);
    return sum;
}

void brutForce(Matrix *m) {
    int dim = m->dimension;
    // Creation des tableaux.
    int *perm = malloc(dim * sizeof(int));
    int *best = malloc(dim * sizeof(int));
    int *worst = malloc(dim * sizeof(int));
    if (!perm || !best || !worst) {
        fprintf(stderr, "Error malloc (brutforce)\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dim; i++){
        perm[i] = i;
    }

    int bestDist = INT_MAX;
    int worstDist = 0;

    //signal(SIGINT, handle_sigint); // Intercepter Ctrl+C

    printf("Calcul de toutes les permutations...\n");
    do {
        int d = totalPathDistance(m, perm, dim);

        if (d < bestDist) {
            bestDist = d;
            memcpy(best, perm, dim * sizeof(int));
        }
        if (d > worstDist) {
            worstDist = d;
            memcpy(worst, perm, dim * sizeof(int));
        }

    } while (nextPermutation(perm, dim));

    printf("\n=== Résultats finaux ===\n");
    printf("Meilleure distance : %d\nChemin : ", bestDist);
    for (int i = 0; i < dim; i++) {
        printf("%d ", best[i]);
    }
    printf("%d\n", best[0]);

    printf("\nPire distance : %d\nChemin : ", worstDist);
    for (int i = 0; i < dim; i++) {
        printf("%d ", worst[i]);
    }
    printf("%d\n", worst[0]);

    free(perm);
    free(best);
    free(worst);
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <tadfile> <nameFile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /* LECTURE DU FICHIER */
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "Cant open the file.\n");
        exit(EXIT_FAILURE);
    }

    Infos* infos = readTsp(f);

    /* Choix du type de fonction */
    int (*fctd)(City*, City*) = NULL;
    if (strcmp(infos->edgeType, "ATT") == 0)
        fctd = distanceAtt;
    else if (strcmp(infos->edgeType, "EUCL_2D") == 0)
        fctd = distanceEucl; // A CHANGER
    else if (strcmp(infos->edgeType, "GEO") == 0)
        fctd = distanceAtt; // A CHANGER
    // Gestion du cas d'erreur.
    if (fctd == NULL) {
        fprintf(stderr, "Edgetype unknown\n");
        // Liberation Memoire
        free(infos->cityArray);
        free(infos);
        exit(EXIT_FAILURE);
    }

    Matrix* m = distanceMatrix(*infos, fctd);
    printMatrix(m);

    brutForce(m);

    freeMatrix(m);
    return 0;
}