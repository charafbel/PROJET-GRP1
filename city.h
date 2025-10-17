#ifndef CITY_H
#define CITY_H


typedef struct city City;
struct city {
    int x;
    int y; // coordonnées GPS
    int id;
};

City* createCity(int id, int xCoor, int yCoor);
void freeCity(City* city);

int getXCoor(City* city);
int getYCoor(City* city);
char* getName(City* city);
#endif //CITY_H
