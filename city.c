#include "city.h"
#include <stdio.h>
#include <stdlib.h>

struct city {
    int x;
    int y; // coordonnées GPS
    char* name;
    int id;
};

// Constructeurs :
City* createCity(char* name, int xCoor, int yCoor){
    City* city = malloc(sizeof(City));
    if (city == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    city->x = xCoor;
    city->y = yCoor;
    city->name = name;
return city;
}

// Getters :
char* getName(City* city) {
    return city->name;
}
int getCoorX(City* city) {
    return city->x;
}
int getCoorY(City* city) {
    return city->y;
}