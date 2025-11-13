#include "matrix.h"
#include "city.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Matrix* MatrixCreate(int dimension) {
    // 1. Allouer de la mémoire pour des POINTEURS DE DOUBLE (sizeof(double*))
    double **mat = malloc(sizeof(double*) * dimension);
    if (!mat) {
        fprintf(stderr, "(MatrixCreate 1) Matrix allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /* Allocation de chaque ligne */
    for (int i = 0; i < dimension; i++){
        // 2. Allouer de la mémoire pour des DOUBLES (sizeof(double))
        mat[i] = malloc(sizeof(double) * (dimension - i - 1));

        // C'est une bonne pratique de vérifier chaque allocation
        if (!mat[i]) {
            fprintf(stderr, "(MatrixCreate Row %d) Matrix allocation failed\n", i);
            // Libérer ce qui a déjà été alloué avant de quitter
            for(int k = 0; k < i; k++) free(mat[k]);
            free(mat);
            exit(EXIT_FAILURE);
        }

        // Initialiser à 0.0 (facultatif mais propre)
        for (int j = 0; j < (dimension - i - 1); j++){
            mat[i][j] = 0.0;
        }
    }

    Matrix* m = malloc(sizeof(Matrix));
    if (!m) {
        fprintf(stderr, "(MatrixCreate 2) Matrix allocation failed\n");
        // Gérer la fuite de mémoire de 'mat' ici
        exit(EXIT_FAILURE);
    }

    m->matrix = mat;
    m->dimension = dimension;
    return m;
}

void freeMatrix(Matrix *m) {
    if (!m) return;
    if (m->matrix) {
        for (int i = 0; i < m->dimension; i++) {
            free(m->matrix[i]); // Libère chaque ligne
        }
        free(m->matrix); // Libère le tableau de pointeurs
    }
    free(m); // Libère la structure
}

void setDistance(Matrix* m, int i, int j, double distance) {
    if (i == j) return;

    if (i > j){
        int temp = i;
        i = j;
        j = temp;
    }

    if (i >= m->dimension || j >= m->dimension) {
        fprintf(stderr, "Index out Of bounds (setDistance)");
        exit(EXIT_FAILURE);
    }

    // Stocke le double
    m->matrix[i][j - i - 1] = distance;
}

double getDistance(Matrix* m, int i, int j) {
    if (i == j) return 0.0; // Renvoie un double

    if (i > j){
        int temp = i;
        i = j;
        j = temp;
    }

    if (i >= m->dimension || j >= m->dimension) {
        fprintf(stderr, "Index out Of bounds (getDistance)");
        exit(EXIT_FAILURE);
    }

    // Renvoie le double
    return m->matrix[i][j - i - 1];
}

