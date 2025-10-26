#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "city.h"
#include "matrix.h"
#define RRR 6378.388  // Rayon approximatif de la Terre en km
#define M_PI 3.1415926535  // Warning de redefinition, probable bug (MACOS)

typedef struct{
    int dimension;
    City** cityArray;
    char edgeType[12];
} Infos ;

/* FCT DE DISTANCES : */

/* Fonctions Géographiques */
double latitude(City* city){
    double deg = city->x;
    double min = city->x - deg;
    return M_PI * (deg + 5.0 * min / 3.0 ) / 180.0;
}
double longitude(City* city) {
    double deg = city->y;
    double min = city->y - deg;
    return M_PI * (deg + 5.0 * min / 3.0 ) / 180.0;
}
int distanceGeo(City* cityA, City* cityB) {
    double latA = latitude(cityA);
    double latB = latitude(cityB);
    double lonA = longitude(cityA);
    double lonB = longitude(cityB);
    double q1 = cos(lonA - lonB);
    double q2 = cos(latA - latB);
    double q3 = cos(latA + latB);
    double dist = RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0;
    return (int)(dist);
}


/* Distances Mathematiques */
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
    /* Fonction ne servant pas directement au projet mais permettant un debugage plus efficace */
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
int canonicalTourLength(Matrix* m) {
    int total = 0;
    int n = m->dimension;
    for (int i = 0; i < n - 1; i++) {
        total += getDistance(m, i, i + 1);
    }
    total += getDistance(m, n - 1, 0); // retour au point de départ
    return total;
}
/* Lecture du fichier tsp */
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
            sscanf(line, "EDGE_WEIGHT_TYPE : %s", infos->edgeType); // De Même pour le type de distances
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
/* Generation de la matrice des distances */
Matrix* distanceMatrix(Infos* infos, int (*fctd)(City*, City*)){ // POINTEUR DE FCT PARAMETRé DANS LE MAIN.
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
