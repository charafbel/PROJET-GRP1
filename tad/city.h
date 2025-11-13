#ifndef CITY_H
#define CITY_H

typedef struct {
    double x;
    double y; // coordonnées GPS
    int id;
}City ;

City* createCity(int id, double xCoor, double yCoor);
void freeCity(City* city);
#endif //CITY_H
