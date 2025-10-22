#include "city.h"
#include <stdio.h>
#include <stdlib.h>

// Constructeurs :
City* createCity(int id, int xCoor, int yCoor){
    City* city = malloc(sizeof(City));
    if (city == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    city->id = id;
    city->x = xCoor;
    city->y = yCoor;
return city;
}
void freeCity(City* city) {
    free(city);
}
// Getters :
int getCoorX(City* city) {
    return city->x;
}
int getCoorY(City* city) {
    return city->y;
}