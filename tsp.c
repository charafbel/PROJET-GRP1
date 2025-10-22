#include "city.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int dimension;
    City** cityArray;
    char edgeType[12];
} Infos ;

/* Version Corrigée de printMatrix */
void printMatrix(Matrix *m) {
    if (m == NULL) {
        printf("Erreur : matrice vide.\n");
        return;
    }
    printf("Matrice de distances (%d x %d) - Triangle supérieur stocké :\n", m->dimension, m->dimension);

    for (int i = 0; i < m->dimension; i++) {
        for (int j = 0; j < m->dimension; j++) {

            // La condition clé est ici :
            if (j >= i) {
                // On est sur ou au-dessus de la diagonale : on affiche la valeur
                printf("%d\t", getDistance(m, i, j));
            } else {
                // On est sous la diagonale : on affiche du vide pour l'alignement
                printf("\t");
            }
        }
        printf("\n"); // Ligne suivante
    }
}

/* FCT DE DISTANCES : */
int distanceAtt(City* cityA, City* cityB) {
    double x = (double)(cityA->x - cityB->x);
    double y = (double)(cityA->y - cityB->y);
    double dist = sqrt( ((x*x)+(y*y))/10 );
    int tij = (int)(dist+0.5);
    if (tij < dist) {
        dist = tij + 1;
    } else {
        dist = tij;
    }
    return dist;
}

/* Lecture et remplissage */
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
    printf("Dimension: %d\n", infos->dimension);
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

    fclose(f);
    return infos;
}

/* Version Corrigée de distanceMatrix */
Matrix* distanceMatrix(Infos infos, int (*fctd)(City*, City*)) { // Passez fctd en paramètre
    printf("[DEBUG] Création de la matrice de distance...\n");

    // CORRECTION : MatrixCreate alloue et retourne déjà la structure Matrix*
    Matrix* m = MatrixCreate(infos.dimension);
    if (!m) {
        perror("MatrixCreate a échoué");
        exit(EXIT_FAILURE);
    }
    // m->dimension est déjà initialisé par MatrixCreate

    /* Remplissage */
    for (int i = 0; i < m->dimension; i++) {
        // On remplit le triangle supérieur (j commence à i)
        for (int j = i; j < m->dimension; j++) {
            City* from = infos.cityArray[i];
            City* to = infos.cityArray[j];
            int dist = fctd(from, to);

            // BUG CORRIGÉ :
            // N'utilisez pas fillMatrix qui utilise les city->id.
            // Utilisez les *indices de boucle* i et j avec setDistance.
            setDistance(m, i, j, dist);
            // printf("[DEBUG] Distance (%d,%d) = %d\n", i, j, dist); // Décommenter si besoin
        }
    }

    printf("[DEBUG] Matrice remplie avec succès.\n");
    return m;
}



/* Version Corrigée de main() */
int main() {
    /* LECTURE DU FICHIER */
    FILE *f = fopen("att15.tsp", "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d’ouvrir le fichier.\n");
        exit(EXIT_FAILURE);
    }
    Infos* infos = readTsp(f);

    /* Choix du type de fonction */
    int (*fctd)(City*, City*) = NULL;
    if (strcmp(infos->edgeType, "ATT") == 0)
        fctd = distanceAtt;
    else if (strcmp(infos->edgeType, "EUCL_2D") == 0)
        fctd = distanceAtt; // A CHANGER
    else if (strcmp(infos->edgeType, "GEO") == 0)
        fctd = distanceAtt; // A CHANGER

    if (fctd == NULL) {
        perror("distanceMatrix: edgeType inconnu");
        // N'oubliez pas de libérer ce qui a été alloué
        free(infos->cityArray); // (supposant que readTsp n'a pas libéré les villes)
        free(infos);
        exit(EXIT_FAILURE);
    }

    // CORRECTION : Passer fctd en paramètre
    Matrix* m = distanceMatrix(*infos, fctd);

    // CORRECTION : Imprimer la matrice ici
    printMatrix(m);


    return 0;
}