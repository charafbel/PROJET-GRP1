#include "matrix.h"
#include "city.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Matrix* MatrixCreate(int dimension) {
    int **mat = malloc(sizeof(int *) * dimension * dimension);
    for (int i = 0; i < dimension; i++) {
        mat[i] = malloc(sizeof(int) * (dimension-i)); // Matrice triangulaire
    }
    Matrix* m = malloc(sizeof(Matrix));
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