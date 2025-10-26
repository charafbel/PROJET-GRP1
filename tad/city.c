#include "city.h"
#include <stdio.h>
#include <stdlib.h>

City* createCity(int id, int xCoor, int yCoor){
    City* city = malloc(sizeof(City));
    if (city == NULL) {
        perror("malloc cityCreate");
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
