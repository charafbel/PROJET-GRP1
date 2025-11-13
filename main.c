#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <getopt.h>


#include "tad/city.h"
#include "tad/matrix.h"
#include "tad/tsp.h"
#include "nearestneighbor.h"
#include "randomwalk.h"



// VARIABLES GLOBALES :
char* methode;
double temps_cpu;
double longueur;
int ij;

/* Ces Variables globales servent a stocker des valeurs d'affichage globales */

/* Structure servant au retour de la fonction brutforce
 * Le but etant d'effectuer l'affichage dans la fonction main.
 */


/* Fonction CTRL + C */
void  INThandler(int sig){
     char  c;
     signal(sig, SIG_IGN);
     // actions...
     printf("arrêt avec i = %d\n",ij);
     printf("OUCH, did you hit Ctrl-C?\n"
            "Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y'){
       // actions
       printf("sortie avec i = %d\n",ij);
       exit(0);
     }
     else{
          printf("Je reprends avec i = %d\n",ij);
          signal(SIGINT, INThandler);
        }
     getchar(); // Get new line character
}

/* Fonctions annexes servant au BrutForce */
void swapArrVal(int *a, int *b) {                       /* Fonction simple d'echange de deux elements dans un tableau */
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
bool nextPermutation(int *arr, int n){     /* Fonction generant la idxutation suivante pour contourner le problème du nombre aberrant de combinaisons possibles */
    int i = n-2; // On part de l'avant dernier element
    // trouver l'indice le plus haut tel que arr[i] < arr[i + 1]
    while (i >= 0 && arr[i] >= arr[i + 1])
        i--;
    if (i < 0)
        return false;
    // trouver le plus petit élément à droite de i qui soit > a arr[i]
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
int totalPathDistance(Matrix *m, int *chemin, int n){ /* Fonction simple servant a calculer la distance d'un chelmin */
    int sum = 0;
    for (int i = 0; i < n - 1; i++){
        sum += getDistance(m, chemin[i], chemin[i + 1]);
    }
    sum += getDistance(m, chemin[n - 1], chemin[0]);
    return sum;
}

/* Force Brute (Algorithme) */
Results* brutForce(Matrix *m) {
    int dim = m->dimension;
    // Creation des tableaux.
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

    int bestDist = INT_MAX; // On initialise la meilleure distance a un valeur ultra elevée et non depassable par le type int

    do { /* Test des permutations et allocation de la meilleure distance */
        int d = totalPathDistance(m, idx, dim);
        if (d < bestDist) {
            bestDist = d;
            ij = bestDist;
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

int main(int argc, char *argv[]){
    signal(SIGINT, INThandler); // Enregistrer le handler
    // TEMPS CPU
    Results* results;
    clock_t start, end;
    double cpu_time_used;
    start = clock();


    if (argc < 2){
        fprintf(stderr, "Usage: %s <tadfile> [options]\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    /* Gestion des FLAGS */
    int opt;
    int help_flag = 0;
    int save_flag = 0;
    int cano_flag = 0;
    char *file_name = NULL;
    char *tsp_file = NULL;
    char *method = NULL;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"file", required_argument, 0, 'f'},
        {"output", required_argument, 0, 'o'},
        {"method", required_argument, 0, 'm'},
        {"print", no_argument, 0, 'c'},
        {0, 0, 0, 0}
    };


    while ((opt = getopt_long(argc, argv, "hf:o:m:c", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                help_flag = 1;
                break;
            case 'f':
                tsp_file = optarg;
                break;
            case 'm':
                if (strcmp(optarg, "bf") == 0)
                    method = "bf";
                if (strcmp(optarg, "nn") == 0)
                    method = "nn";
                if (strcmp(optarg, "rw") == 0)
                method = "rw";
                if(method == NULL){
                    fprintf(stderr, "Error unknown method :  %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                save_flag = 1;
                file_name = optarg;
                break;
            case 'c':
                cano_flag = 1;
                break;
            default:
                fprintf(stderr, "Incorrect usage. Type -h for help.\n");
                exit(EXIT_FAILURE);
        }
    }
    if (!method) {
        fprintf(stderr, "Error: missing -m <method> (e.g., bf)\n");
        exit(EXIT_FAILURE);
    }
    if (help_flag) {
        printf("Usage: %s <tadfile> [options]\n", argv[0]);
        printf("Options:\n");
        printf("  -h, --help           Print this help message\n");
        printf("  -f [file]             TSP FILE TO READ\n");
        printf("  -c                       Print result");
        printf("  -o [output]              Save output to a file (append mode). Default file: default.txt\n");
        printf("  -m method, --m method    Calculation method (bf for brute force)\n");
        return 0;
    }

    if (tsp_file == NULL) {
        fprintf(stderr, "Error: missing required <tadfile> argument\n");
        exit(EXIT_FAILURE);
    }
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
        fctd = distanceAtt;
    if (strcmp(infos->edgeType, "EUCL_2D") == 0)
        fctd = distanceEucl;
    if (strcmp(infos->edgeType, "GEO") == 0)
        fctd = distanceGeo;
    if (fctd == NULL) {
        fprintf(stderr, "Edgetype unknown\n");
        free(infos->cityArray);
        free(infos);
        exit(EXIT_FAILURE);
    }

    methode = infos->edgeType;

    Matrix* m = distanceMatrix(infos, fctd);

    if (method && strcmp(method, "bf") == 0) {
        results = brutForce(m);
    }
    else if (method && strcmp(method, "nn") == 0) {
        results = nearestNeighbour(m,9);
    }
    else if (method && strcmp(method, "rw") == 0) {
        results = randomWalk(m);}
    else {
        fprintf(stderr, "Method not implemented or specified.\n");
        return 1;
    }

    /* Vu que les fichiers tsp sont dans un fichier, pour l'affichage il faut eviter l'affichage du ./tsp/ */

    char *fn = strrchr(tsp_file, '/');
    fn++;
    end = clock();
    cpu_time_used = ((double) (end - start))/CLOCKS_PER_SEC;

    /* Ecriture si save_flag actif */
    if (save_flag) {
        FILE *out = fopen(file_name, "a");
        if (out == NULL){
            fprintf(stderr, "Error opening file %s for writing\n", file_name);
            exit(EXIT_FAILURE);
        }
        fprintf(out, "%s ; %s ; %f ; %d ; [", fn, method, cpu_time_used, results->bestDistance);
        for (int i = 0; i < results->dimension; i++) {
            fprintf(out, "%d", results->bestPath[i]);
            if (i != results->dimension-1) {
                fprintf(out, ",");
            }
        }
        fprintf(out, "]\n");
        fclose(out);
    }
    if (cano_flag) {
        printf("%d",canonicalTourLength(m));
    } else {
        /* Affichage console */
        printf("Instance ; Méthode ; Temps CPU (sec) ; Longueur ; Tour\n");
        printf("%s ; %s ; %f ; %d ; [", fn, method, cpu_time_used, results->bestDistance);
        for (int i = 0; i < results->dimension; i++) {
            printf("%d", results->bestPath[i] + 1);
            if (i != results->dimension-1) {
                printf(",");
            }
        }
        printf("]\n");
    }

    /* Liberation Memoire */
    freeMatrix(m);
    free(infos->cityArray);
    free(infos);
    free(results->bestPath);
    free(results);
    return 0;
}
