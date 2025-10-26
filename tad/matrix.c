#include "matrix.h"
#include "city.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Matrix* MatrixCreate(int dimension) {
    int **mat = malloc(sizeof(int*) * dimension);
    if (!mat) {
        fprintf(stderr, "(MatrixCreate 1) Matrix allocation failed\n");
        exit(EXIT_FAILURE);
    }
    /* Allocation de chaque ligne en enlevant un element a chaque ligne pour avoir une matrice triangulaire supérieure */
    for (int i = 0; i < dimension; i++){
        mat[i] = malloc(sizeof(int) * (dimension - i - 1));
        for (int j = 0; j < (dimension - i - 1); j++){
            mat[i][j] = 0;
        }
    }
    Matrix* m = malloc(sizeof(Matrix));
    if (!m) {
        fprintf(stderr, "(MatrixCreate 2) Matrix allocation failed\n");
        exit(EXIT_FAILURE);
    }

    m->matrix = mat;
    m->dimension = dimension;
    return m;
}
void freeMatrix(Matrix *m) {
    for (int i = 0; i < m->dimension; i++) {
        free(m->matrix[i]);
    }
    free(m->matrix);
    free(m);
}

void setDistance(Matrix* m, int i, int j, int distance) {
    if (i == j) return; // On ignore la diagonale.

    // Gestion de la symetrie
    if (i > j){
        int temp = i;
        i = j;
        j = temp;
    }
    // Verification des bornes
    if (i >= m->dimension || j >= m->dimension) {
        fprintf(stderr, "Index out Of bounds (setDistance)");
        exit(EXIT_FAILURE);
    }
    m->matrix[i][j - i - 1] = distance;
}
int getDistance(Matrix* m, int i, int j) {
    if (i == j) return 0;

    // Gestion de la symetrie
    if (i > j){
        int temp = i;
        i = j;
        j = temp;
    }
    // Verification des bornes
    if (i >= m->dimension || j >= m->dimension) {
        fprintf(stderr, "Index out Of bounds (getDistance)");
        exit(EXIT_FAILURE);
    }
    return m->matrix[i][j - i - 1];
}
