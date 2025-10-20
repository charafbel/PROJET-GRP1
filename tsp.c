#include "city.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

int main() {
    FILE *f = fopen("att15.tsp", "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d’ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }

    City **cities = readTsp(f); // ton readTsp renvoie un tableau de City*

    printf("=== Liste des villes lues ===\n");
    for (int i = 0; i < 15; i++) {
        printf("Ville %d : ID = %d, x = %d, y = %d\n",
               i + 1, cities[i]->id, cities[i]->x, cities[i]->y);
    }
    free(cities);
    return 0;
}