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
#include "brutforce.h"
#include "ga.h"

char* methode;
double temps_cpu;
double longueur;
int ij;

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

void printOutputFile(char* file_name, Results* results, Infos* infos, char* fn, char* method, double cpu_time_used){
    FILE *out = fopen(file_name, "a");
    if (out == NULL){
        fprintf(stderr, "Error opening file %s for writing\n", file_name);
        exit(EXIT_FAILURE);
    }
    if (strcmp(infos->edgeType, "EUC_2D") == 0) {
        fprintf(out, "%s ; %s ; %f ; %f ; [", fn, method, cpu_time_used, results->bestDistance);
    } else {
        fprintf(out, "%s ; %s ; %f ; %g ; [", fn, method, cpu_time_used, results->bestDistance);
    }
    for (int i = 0; i < results->dimension; i++) {
        fprintf(out, "%d", results->bestPath[i]);
        if (i != results->dimension-1) {
            fprintf(out, ",");
        }
    }
    fprintf(out, "]\n");
    fclose(out);
}
void printOutput(Results* results, Infos* infos, char* fn, char* method, double cpu_time_used){
    printf("Instance ; Méthode ; Temps CPU (sec) ; Longueur ; Tour\n");
    if (strcmp(infos->edgeType, "EUC_2D") == 0) {
        printf("%s ; %s ; %f ; %f ; [", fn, method, cpu_time_used, results->bestDistance);
    } else {
        printf("%s ; %s ; %f ; %g ; [", fn, method, cpu_time_used, results->bestDistance);
    }
    for (int i = 0; i < results->dimension; i++) {
        printf("%d", results->bestPath[i] + 1);
        if (i != results->dimension-1) {
            printf(",");
        }
    }
    printf("]\n");
}



int main(int argc, char *argv[]){
    signal(SIGINT, INThandler);
    Results* results;
    clock_t start, end;
    double cpu_time_used;

    if (argc < 2){
        fprintf(stderr, "Usage: %s <tadfile> [options]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    start = clock();

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
    else if (strcmp(optarg, "nn") == 0)
        method = "nn";
    else if (strcmp(optarg, "rw") == 0)
        method = "rw";
    else if (strcmp(optarg, "2optrw") == 0)
        method = "2optrw";
    else if (strcmp(optarg, "ga") == 0)
        method = "ga";
    else {
        fprintf(stderr, "Error unknown method : %s\n", optarg);
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

    // Verifications
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


    // Executioon
    FILE *tsp = fopen(tsp_file, "r");
    if (tsp == NULL) {
        fprintf(stderr, "(Open TSP) Cant open the file.\n");
        exit(EXIT_FAILURE);
    }

    Infos* infos = readTsp(tsp);

    /* Choix du type de fonction */
    double (*fctd)(City*, City*) = NULL;
    if (strcmp(infos->edgeType, "ATT") == 0)
        fctd = distanceAtt;
    if (strcmp(infos->edgeType, "EUC_2D") == 0)
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
        results = randomWalk(m);
    }
    else if (method && strcmp(method, "2optrw") == 0) {
        results = randomWalk(m);
        results = twoOptrw(m, results);
    }
    else if (method && strcmp(method, "ga") == 0) {
    results = geneticAlgorithm(m);
    }
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
        printOutputFile(file_name, results, infos, fn, method, cpu_time_used);
    }
    if (cano_flag) {
        printf("%f",canonicalTourLength(m));
    } else {
        printOutput(results, infos, fn, method, cpu_time_used);
    }

    /* Liberation Memoire */
    freeMatrix(m);
    free(infos->cityArray);
    free(infos);
    free(results->bestPath);
    free(results);
    return 0;
}
