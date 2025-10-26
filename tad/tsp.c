#define _USE_MATH_DEFINES  // EN PREMIER !
#include <math.h>          // Juste après
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "city.h"          // Tes .h viennent APRES
#include "matrix.h"
#define RRR 6378.388  // Rayon approximatif de la Terre en km
#define M_PI 3.1415926535  // Valeur de Pi


// ... LA SUITE DE TON CODE (typedef struct Infos...) ...

typedef struct {
    int dimension;
    City** cityArray;
    char edgeType[12];
} Infos ;



/* FCT DE DISTANCES : */
double deg2rad(double deg) {
    return deg * M_PI/180.0;
}
// Conversion spéciale TSP (degrés, minutes) vers radians
double latitude(City* city) {
    double deg = (int)(city->x);
    double min = city->x - deg;
    double lat = M_PI * (deg + 5.0 * min / 3.0) / 180.0;
    return lat;
}
double longitude(City* city) {
    double deg = (int)(city->y);
    double min = city->y - deg;
    double lon = M_PI * (deg + 5.0 * min / 3.0) / 180.0;
    return lon;
}
int distanceGeo(City* cityA, City* cityB) {
    double latA = latitude(cityA);
    double lonA = longitude(cityA);
    double latB = latitude(cityB);
    double lonB = longitude(cityB);
    double q1 = cos(lonA - lonB);
    double q2 = cos(latA - latB);
    double q3 = cos(latA + latB);
    double dist = RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0;
    return (int)(dist);
}
int distanceAtt(City* cityA, City* cityB) {
    double xd = (double)(cityA->x - cityB->x);
    double yd = (double)(cityA->y - cityB->y);

    double rij = sqrt((xd * xd + yd * yd) / 10.0);
    int tij = (int)(rij + 0.5);
    if (tij < rij)
        tij++;

    return tij;
}
int distanceEucl(City* cityA, City* cityB){
    double x_a = cityA->x;
    double y_a = cityA->y;
    double x_b = cityB->x;
    double y_b = cityB->y;
    double x_offset = x_b - x_a;
    double y_offset = y_b - y_a;
    double dist = sqrt(x_offset * x_offset + y_offset * y_offset);

    return (int)(dist + 0.5);
}

/* Lecture et remplissage */
void printMatrix(Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "Matrix is NULL\n");
        exit(EXIT_FAILURE);
    }

    printf("Matrice de distances : \n");
    for (int i = 0; i < m->dimension; i++) {
        for (int j = 0; j < m->dimension; j++) {
            if (j >= i){
                // On est sur ou au-dessus de la diagonale : on affiche la valeur
                printf("%d\t", getDistance(m, i, j));
            } else{
                // On est sous la diagonale : on affiche du vide pour l'alignement
                printf("\t");
            }
        }
        printf("\n");
    }
}
Infos* readTsp(FILE *f){
    char line[1024];
    Infos* infos = malloc(sizeof(Infos));
    infos->dimension = 0;
    strcpy(infos->edgeType, "");

    // Boucle de lecture TSP :
    while (fgets(line, 1024, f)) {
        if (strncmp(line, "DIMENSION : ", 12) == 0) {
            sscanf(line, "DIMENSION : %d ", &(infos->dimension)); // On capture la valeur de DIMENSION SELON LE MODELE DU FICHIER
        }
        if (strncmp(line, "EDGE_WEIGHT_TYPE", 16) == 0){
            sscanf(line, "EDGE_WEIGHT_TYPE : %s", infos->edgeType);
        }
        // On s'arrete une fois avoir depassé la ligne NODE_COORD_SECTION
        if (strncmp(line, "NODE_COORD_SECTION", strlen("NODE_COORD_SECTION")) == 0) {
            fgets(line, 1024, f);
            break; // Pour aller dans la boucle de lecture des coordonnées
        }
    }

    if (infos->dimension <= 0)// Prevenir l'erreur d'un tableau vide ou negatif.
        exit(1);
    infos->cityArray = malloc(infos->dimension* sizeof(City*));
    //printf("Dimension: %d\n", infos->dimension);

    /* Lecture de toutes les villes selon le schéma TSP */
    for (int i = 0; i < infos->dimension; i++) {
        int id = 0;
        int x= 0;
        int y= 0;
        City* city;
        sscanf(line, "%d %d %d", &id, &x, &y);
        city = createCity(id, x, y);
        fgets(line, 1024, f);
        infos->cityArray[i] = city;
    }

    /* Terminaison */
    fclose(f);
    return infos;
}
Matrix* distanceMatrix(Infos* infos, int (*fctd)(City*, City*)){
    Matrix* m = MatrixCreate(infos->dimension);
    if (!m) {
        fprintf(stderr, "MatrixCreate failed\n");
        exit(EXIT_FAILURE);
    }

    /* Remplissage */
    for (int i = 0; i < infos->dimension; i++) {
        for (int j = i + 1; j < infos->dimension; j++) {
            int dist = fctd(infos->cityArray[i], infos->cityArray[j]);
            setDistance(m, i, j, dist);
        }
    }
    return m;
}

