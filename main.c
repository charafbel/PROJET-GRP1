#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <signal.h>
#include <string.h>

#include "tad/city.h"
#include "tad/matrix.h"
#include "tad/tsp.h"

// VARIABLES GLOBALES :

char* instance_name;
char* methode;
double temps_cpu;
double longueur;





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
    if (!perm || !best) {
        fprintf(stderr, "Error malloc (brutforce)\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dim; i++){
        perm[i] = i;
    }

    int bestDist = INT_MAX;
    //signal(SIGINT, handle_sigint); // Intercepter Ctrl+C


    do {
        int d = totalPathDistance(m, perm, dim);

        if (d < bestDist) {
            bestDist = d;
            memcpy(best, perm, dim * sizeof(int));
        }
    } while (nextPermutation(perm, dim));


    printf("%s ; %s ; %f ; %d ; [", instance_name, methode, 0.00, bestDist);
    for (int i = 0; i < dim; i++) {
        printf("%d", best[i]);
        if (i != dim-1) {
            printf(",");
        }
    }
    printf("]\n");

    free(perm);
    free(best); 
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

    methode = infos->edgeType;
    instance_name = argv[1];
    // temps_cpu


    Matrix* m = distanceMatrix(*infos, fctd);
    brutForce(m);

    freeMatrix(m);
    return 0;
}