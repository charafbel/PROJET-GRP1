#include "city.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int dimension;
    City** cityArray = malloc(dimension * sizeof(City*));
    char edgeType[12] = "";
} Infos ;

Infos* readTsp(FILE *f){
    char line[1024];
    Infos* infos = (Infos*)malloc(sizeof(Infos));
    infos->dimension = 0;

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

int main() {
    /* LECTURE DU FICHIER */
    FILE *f = fopen("att15.tsp", "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d’ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }



    return 0;
}