#include "city.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Dic* readTsp(FILE *f) {
    char line[1024];
    int dimension;
    int id;
    int x;
    int y;
    Dic* dic = createDictionary();

    while (fgets(line, 1024, f)) {
        printf("Ligne lue: '%s'\n", line);
        // AI
        if (strncmp(line, "DIMENSION", 9) == 0) {
            // Mieux parser la dimension (chaîne plus souple)
            char* p = strchr(line, ':');
            if (p)
                sscanf(p + 1, "%d", &dimension);
            printf("Dimension: %d\n", dimension);
        }
        if (strncmp(line, "NODE_COORD_SECTION", 1) == 0) {
            printf("BREAK");
            break; // Pour aller dans la boucle de lecture des coordonnées
        }
    }
    printf("Dimension: %d\n", dimension);
    for (int i = 0; i < dimension; i++) {
        City* city;
        sscanf(line, "%d %d %d", &id, &x, &y);
        printf("%d %d %d\n", id, x, y);
        city = createCity(id, x, y);
        printf("VILLE = %d, %d, %d", city->x, city->y, city->id);
        dic = addToDictionary(dic, city);
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
    // printDictionary(dic);
    return 0;
}