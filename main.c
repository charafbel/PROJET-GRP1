#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <getopt.h>

#include "tad/city.h"
#include "tad/matrix.h"
#include "tad/tsp.h"



// VARIABLES GLOBALES :
char* instance_name;
char* methode;
double temps_cpu;
double longueur;

typedef struct {
    int dimension;
    int bestDistance;
    int *bestPath;
}Results;


void swapArrVal(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

bool nextPermutation(int *arr, int n){
    int i = n-2;
    while (i >= 0 && arr[i] >= arr[i + 1])
        i--;
    if (i < 0)
        return false;
    // Étape 2 : trouver le plus petit élément à droite de i qui soit > arr[i]
    int j = n-1;
    while (arr[j] <= arr[i])
        j--;
    // Étape 3 : échanger les deux
    swapArrVal(&arr[i], &arr[j]);
    // Étape 4 : inverser la fin du tableau (partie après i)
    int k = i + 1;
    int l = n - 1;
    while (k < l){
        swapArrVal(&arr[k], &arr[l]);
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

Results* brutForce(Matrix *m) {
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

    Results* results = malloc(sizeof(Results));
    results->dimension = dim;
    results->bestDistance = bestDist;
    results->bestPath = malloc(dim * sizeof(int));
    memcpy(results->bestPath, best, dim * sizeof(int));

    free(perm);
    free(best);
    return results;
}

int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(stderr, "Usage: %s <tadfile> [options]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Gestion des FLAGS */
    int opt;
    int help_flag = 0;
    int save_flag = 0;
    char *file_name = NULL;
    char *method = NULL;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"f", optional_argument, 0, 'f'},
        {"o", optional_argument, 0, 'o'},
        {"m", required_argument, 0, 'm'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "hf::o::m:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                help_flag = 1;
                break;
            case 'f':
                save_flag = 1;
                file_name = "results.txt";
                break;
            case 'm':
                if (strcmp(optarg, "bf") == 0) {
                    method = "bf";
                } else {
                    fprintf(stderr, "Error unknown method :  %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Incorrect usage. Type -h for help.\n");
                exit(EXIT_FAILURE);
        }
    }

    if (help_flag) {
        printf("Usage: %s <tadfile> [options]\n", argv[0]);
        printf("Options:\n");
        printf("  -h, --help           Print this help message\n");
        printf("  -f [file], --f [file]    Save output to a file (append mode). Default file: default.txt\n");
        printf("  -m method, --m method    Calculation method (e.g., bf for brute force)\n");
        return 0;
    }
    if (method != NULL) {
        //printf("Method chosen: %s\n", method);
    }

    if (optind >= argc) {
        fprintf(stderr, "Error: missing required <tadfile> argument\n");
        exit(EXIT_FAILURE);
    }
    char *tsp_file = argv[optind];

    /* ------- EXECUTION DU CODE --------- */

    FILE *tsp = fopen(tsp_file, "r");
    if (tsp == NULL) {
        fprintf(stderr, "(Open TSP) Cant open the file.\n");
        exit(EXIT_FAILURE);
    }

    Infos* infos = readTsp(tsp);

    /* Choix du type de fonction */
    int (*fctd)(City*, City*) = NULL;
    if (strcmp(infos->edgeType, "ATT") == 0)
        fctd = distanceAtt; /* Inutile d'inclure les fonctions futures pour l'instant */
    if (fctd == NULL) {
        fprintf(stderr, "Edgetype unknown\n");
        free(infos->cityArray);
        free(infos);
        exit(EXIT_FAILURE);
    }

    methode = infos->edgeType;
    instance_name = argv[1];

    Matrix* m = distanceMatrix(*infos, fctd);
    Results* results;
    if (method && strcmp(method, "bf") == 0) {
        results = brutForce(m);
    } else {
        fprintf(stderr, "Method not implemented or specified.\n");
    }

    char *fn = strrchr(tsp_file, '/');
    fn++;

    if (save_flag) {
        //printf("Save to file : %s\n", file_name);
        FILE *out = fopen(file_name, "w");
        if (out == NULL){
            fprintf(stderr, "Error opening file %s for writing\n", file_name);
            exit(EXIT_FAILURE);
        }
/*
        fprintf(out, "%s ; %s ; %f ; %d ; [", fn, method, 0.00, results->bestDistance);
        for (int i = 0; i < results->dimension; i++) {
            fprintf(out, "%d", results->bestPath[i]);
            if (i != results->dimension-1) {
                fprintf(out, ",");
            }
        }
        fprintf(out, "]");
        close(out);
    } else {
*/
    }
    printf("Instance ; Méthode ; Temps CPU (sec) ; Longueur ; Tour\n");
    printf("%s ; %s ; %f ; %d ; [", fn, method, 0.00, results->bestDistance);
    for (int i = 0; i < results->dimension; i++) {
        printf("%d", results->bestPath[i] + 1);
        if (i != results->dimension-1) {
            printf(",");
        }
    }
    printf("]\n");

    /* Liberation Memoire */

    freeMatrix(m);
    free(infos->cityArray);
    free(infos);
    free(results->bestPath);
    free(results);
    return 0;
}
