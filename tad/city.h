#ifndef CITY_H
#define CITY_H

typedef struct {
    int x;
    int y; // coordonnées GPS
    int id;
}City ;

City* createCity(int id, int xCoor, int yCoor);
void freeCity(City* city);
#endif //CITY_H
