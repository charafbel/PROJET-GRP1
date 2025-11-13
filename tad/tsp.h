#ifndef TSP_H
#define TSP_H

#include "city.h"
#include "matrix.h"
#include <stdio.h>

/* Structure servant au retour et au stockage des informations lors de la lecture */
typedef struct {
    int dimension;
    City** cityArray;
    char edgeType[12];   // Type de distance (EUCL_2D, ATT, GEO, etc.)
} Infos;

/* Fonctions de lecture et de traitement */
Infos* readTsp(FILE *f);
Matrix* distanceMatrix(Infos* infos, double (*fctd)(City*, City*));
void printMatrix(Matrix *m);

/* Fonctions de distance */
double distanceAtt(City* cityA, City* cityB);
double distanceEucl(City* cityA, City* cityB);
double distanceGeo(City* cityA, City* cityB);
double canonicalTourLength(Matrix* m);

/* Fonctions utilitaires pour le mode GEO */
double latitude(City* city);
double longitude(City* city);

#endif