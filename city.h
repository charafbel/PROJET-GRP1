#ifndef CITY_H
#define CITY_H

#include "dictionary.h"

typedef struct city City;

City* createCity(char* name, int xCoor, int yCoor);
void freeCity(City* city);

int getXCoor(City* city);
int getYCoor(City* city);
char* getName(City* city);
#endif //CITY_H
