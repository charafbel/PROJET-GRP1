#include "city.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int dimension;
    City** cityArray;
    char edgeType[12];
} Infos ;

void printMatrix(Matrix* m) {
    if (m == NULL) {
        printf("Erreur : matrice vide.\n");
        return;
    }
    printf("Matrice de distances (%d x %d) :\n", m->dimension, m->dimension);
    for (int i = 0; i < m->dimension; i++) {
        for (int j = 0; j < m->dimension; j++) {
            printf("%d ", m->matrix[i][j]);
        }
        printf("\n");
    }
}

/* FCT DE DISTANCES : */
int distanceAtt(City* cityA, City* cityB) {
    double x = (double)(cityA->x - cityB->x);
    double y = (double)(cityA->y - cityB->y);
    double dist = sqrt( ((x*x)+(y*y))/10 );
    int tij = (int)(dist+0.5);
    if (tij < dist) {
        dist = tij + 1;
    } else {
        dist = tij;
    }
    return dist;
}

/* Lecture et remplissage */
Infos* readTsp(FILE *f){
    char line[1024];
    Infos* infos = malloc(sizeof(Infos));
    infos->cityArray = malloc(infos->dimension * sizeof(City*));
    infos->dimension = 0;
    strcpy(infos->edgeType, "");

    // Boucle de lecture TSP :
    while (fgets(line, 1024, f)) {
        if (strncmp(line, "DIMENSION : ", 12) == 0) {
            sscanf(line, "DIMENSION : %d ", &(infos->dimension)); // On capture la valeur de DIMENSION SELON LE MODELE DU FICHIER
        }
        if (strncmp(line, "EDGE_WEIGHT_TYPE", 16) == 0){
            sscanf(line, "EDGE_WEIGHT_TYPE : %s", infos->edgeType);
        }
        // On s'arrete une fois avoir depassé la ligne NODE_COORD_SECTION
        if (strncmp(line, "NODE_COORD_SECTION", strlen("NODE_COORD_SECTION")) == 0) {
            fgets(line, 1024, f);
            break; // Pour aller dans la boucle de lecture des coordonnées
        }
    }

    if (infos->dimension <= 0)// Prevenir l'erreur d'un tableau vide ou negatif.
        exit(1);

    printf("Dimension: %d\n", infos->dimension);
    for (int i = 0; i < infos->dimension; i++) {
        int id = 0;
        int x= 0;
        int y= 0;
        City* city;
        sscanf(line, "%d %d %d", &id, &x, &y);
        city = createCity(id, x, y);
        fgets(line, 1024, f);
        infos->cityArray[i] = city;
    }

    fclose(f);
    return infos;
}
Matrix* distanceMatrix(Infos infos) {
    /* Init */
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    int (*fctd)(City*, City*) = NULL;
    m->dimension = infos.dimension;

    /* Choix du type de fonction */
    if (strcmp(infos.edgeType, "ATT") == 0)
        fctd = distanceAtt;
    if (strcmp(infos.edgeType, "EUCL_2D") == 0)
        fctd = distanceAtt; // A CHANGER
    if (strcmp(infos.edgeType, "GEO") == 0)
        fctd = distanceAtt; // A CHANGER
    if (fctd == NULL) {
        perror("Erreur de lecture");
        exit(EXIT_FAILURE);
    }

    /* Remplissage */
    for (int i = 0; i < infos.dimension; i++) {
        for (int j = i+1; j < infos.dimension; j++) {
            City* from = infos.cityArray[i];
            City* to = infos.cityArray[j];
            fillMatrix(m, from, to, fctd(from, to));
        }
    }
    printMatrix(m);
    return m;
}

int main() {
    /* LECTURE DU FICHIER */
    FILE *f = fopen("att15.tsp", "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d’ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }
    Infos* infos = readTsp(f);
    Matrix* m = distanceMatrix(*infos);
    return 0;
}