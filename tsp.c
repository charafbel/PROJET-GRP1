#include "city.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


City** readTsp(FILE *f){
    char line[1024];
    int dimension;

    // Boucle de lecture TSP :
    while (fgets(line, 1024, f)) {
        if (strncmp(line, "DIMENSION : ", 12) == 0) {
            sscanf(line, "DIMENSION : %d ", &dimension); // On capture la valeur de DIMENSION SELON LE MODELE DU FICHIER
        }
        // On s'arrete une fois avoir depassé la ligne NODE_COORD_SECTION
        if (strncmp(line, "NODE_COORD_SECTION", strlen("NODE_COORD_SECTION")) == 0) {
            fgets(line, 1024, f);
            break; // Pour aller dans la boucle de lecture des coordonnées
        }
    }

    if (dimension <= 0)// Prevenir l'erreur d'un tableau vide ou negatif.
        exit(1);
    City** cityArray = malloc(dimension * sizeof(City*));

    printf("Dimension: %d\n", dimension);
    for (int i = 0; i < dimension; i++) {
        int id = 0;
        int x= 0;
        int y= 0;
        City* city;
        sscanf(line, "%d %d %d", &id, &x, &y);
        city = createCity(id, x, y);
        fgets(line, 1024, f);
        cityArray[i] = city;
    }

    fclose(f);
    return cityArray;
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

    /* RECUPERATION DU TYPE DE DISTANCES */
    char li[1024];
    char edgeType[64] = "";
    while (fgets(li, 1024, f)) {
        if (strncmp(li, "EDGE_WEIGHT_TYPE", 16) == 0){
            sscanf(li, "EDGE_WEIGHT_TYPE : %s", edgeType);
            break;
        }
    }

    /* LECTURE TSP */
    rewind(f); // Afin de revenir au depart pour une autre lecture.
    City **cities = readTsp(f); // readTsp renvoie un tableau de City*

    /* Affichage des Informations nécessaires */
    printf("EDGE_WEIGHT_TYPE : %s\n", edgeType);
    printf("=== Liste des villes lues ===\n");
    for (int i = 0; i < 15; i++) {
        printf("Ville %d : ID = %d, x = %d, y = %d\n",i + 1, cities[i]->id, cities[i]->x, cities[i]->y);
    }

    /* Terminaison */
    free(cities);
    return 0;
}