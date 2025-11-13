#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "city.h"     // Doit utiliser des double pour City->x, City->y
#include "matrix.h"   // Doit utiliser des double pour la matrice
#define RRR 6378.388  // Rayon approximatif de la Terre en km
#define M_PI 3.1415926535

typedef struct{
    int dimension;
    City** cityArray;
    char edgeType[12];
} Infos ;

// Définition du type de pointeur de fonction que nous utiliserons
// Il attend une fonction qui prend deux City* et renvoie un double.
typedef double (*fctd)(City*, City*);

double latitude(City* city) {
    return M_PI * city->x / 180.0;
}

double longitude(City* city) {
    return M_PI * city->y / 180.0;
}

double distanceGeo(City* cityA, City* cityB) {
    double latA = latitude(cityA);
    double latB = latitude(cityB);
    double lonA = longitude(cityA);
    double lonB = longitude(cityB);
    double q1 = cos(lonA - lonB);
    double q2 = cos(latA - latB);
    double q3 = cos(latA + latB);
    double arg = 0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3);
    if (arg > 1.0) arg = 1.0;
    if (arg < -1.0) arg = -1.0;
    double dab = RRR * acos(arg);
    return floor(dab + 0.5);
}

double distanceEucl(City* cityA, City* cityB) {
    double xd = cityA->x - cityB->x;
    double yd = cityA->y - cityB->y;
    double dist = sqrt(xd * xd + yd * yd);
    return dist;  // ou return round(dist);
}

double distanceAtt(City* cityA, City* cityB) {
    double xd = (double)(cityA->x - cityB->x);
    double yd = (double)(cityA->y - cityB->y);

    double rij = sqrt((xd * xd + yd * yd) / 10.0);
    int tij = (int)(rij + 0.5); // Arrondi à l'entier
    if (tij < rij)
        tij++;

    // Renvoie l'entier casté en double (ex: 123.0)
    return tij;
}
/* ====================================================================== */
/* Lecture et remplissage (Mis à jour pour double) */
/* ====================================================================== */

void printMatrix(Matrix *m) {
    if (m == NULL) {
        fprintf(stderr, "Matrix is NULL\n");
        exit(EXIT_FAILURE);
    }

    printf("Matrice de distances : \n");
    for (int i = 0; i < m->dimension; i++) {
        for (int j = 0; j < m->dimension; j++) {
            if (j >= i){
                // !! MODIFIÉ !! : %g gère les .0 et les .75
                printf("%g\t", getDistance(m, i, j));
            } else{
                printf("\t");
            }
        }
        printf("\n");
    }
}

double canonicalTourLength(Matrix* m) {
    double total = 0.0;
    int n = m->dimension;
    for (int i = 0; i < n - 1; i++) {
        total += getDistance(m, i, i + 1);
    }
    total += getDistance(m, n - 1, 0);
    return total;
}

/* Lecture du fichier tsp */
Infos* readTsp(FILE *f){
    char line[1024];
    Infos* infos = malloc(sizeof(Infos));
    infos->dimension = 0;
    strcpy(infos->edgeType, "");
    char key[100];
    char value[100];

    // Boucle de lecture TSP (ROBUSTE) :
    while (fgets(line, 1024, f)) {
        if (strncmp(line, "NODE_COORD_SECTION", 18) == 0) {
            fgets(line, 1024, f);
            break;
        }
        if (sscanf(line, " %99[^: \t] : %99s", key, value) == 2) {
            for (int i = strlen(key) - 1; i >= 0 && (key[i] == ' ' || key[i] == '\t'); i--) {
                key[i] = '\0';
            }
            if (strcmp(key, "DIMENSION") == 0) {
                infos->dimension = atoi(value);
            }
            else if (strcmp(key, "EDGE_WEIGHT_TYPE") == 0) {
                strncpy(infos->edgeType, value, 11);
                infos->edgeType[11] = '\0';
            }
        }
    }
    if (infos->dimension <= 0) {
        fprintf(stderr, "Erreur: Dimension non lue ou invalide (%d)\n", infos->dimension);
        exit(1);
    }
    infos->cityArray = malloc(infos->dimension * sizeof(City*));

    // Lecture des coordonnées (correcte, lit des doubles)
    for (int i = 0; i < infos->dimension; i++) {
        int id = 0;
        double x = 0.0;
        double y = 0.0;
        City* city;

        if (sscanf(line, "%d %lf %lf", &id, &x, &y) != 3) {
            if (strncmp(line, "EOF", 3) == 0) break;
            fgets(line, 1024, f);
            i--;
            continue;
        }

        // createCity doit accepter (int, double, double)
        city = createCity(id, x, y);

        fgets(line, 1024, f);
        infos->cityArray[i] = city;
    }
    fclose(f);
    return infos;
}

/* Generation de la matrice des distances */
Matrix* distanceMatrix(Infos* infos, fctd fctd){ // <-- Utilise le type double
    Matrix* m = MatrixCreate(infos->dimension);
    if (!m) {
        fprintf(stderr, "MatrixCreate failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < infos->dimension; i++) {
        for (int j = i + 1; j < infos->dimension; j++) {
            // fctd() renvoie un double
            double dist = fctd(infos->cityArray[i], infos->cityArray[j]);
            // setDistance doit accepter un double
            setDistance(m, i, j, dist);
        }
    }
    return m;
}