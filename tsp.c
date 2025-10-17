#include "city.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Dic* readTsp(FILE *f) {
    char line[1024];
    int dimension;
    Dic* dic = createDictionary();

    while (fgets(line, 1024, f)) {
        if (strncmp(line, "DIMENSION : ", 12) == 0) {
            sscanf(line, "DIMENSION : %d ", &dimension);
        }
        if (strncmp(line, "NODE_COORD_SECTION", strlen("NODE_COORD_SECTION")) == 0) {
            fgets(line, 1024, f);
            break; // Pour aller dans la boucle de lecture des coordonnées
        }
    }
    printf("Dimension: %d\n", dimension);
    for (int i = 0; i < dimension; i++) {
        int id = 0;
        int x= 0;
        int y= 0;
        City* city;
        sscanf(line, "%d %d %d", &id, &x, &y);
        city = createCity(id, x, y);
        dic = addToDictionary(dic, city);
        fgets(line, 1024, f);
    }
    fclose(f);
    return dic;

}

void printDictionary(Dic* dic) {
    printf("Dictionnaire contient %d villes :\n", dic->size);
    for (int i = 0; i < dic->size; i++) {
        City* city = dic->dic[i].city;
        printf("ID: %d, x: %d, y: %d\n", city->id, city->x, city->y);
    }
}

int main() {
    FILE *f = fopen("att15.tsp", "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }
    Dic* dic = readTsp(f);
    printDictionary(dic);
    return 0;
}