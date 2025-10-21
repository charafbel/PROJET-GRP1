#ifndef MATRIX_H
#define MATRIX_H

#include "city.h"

typedef struct {
    int dimension;
    int **matrix;
} Matrix;

Matrix* MatrixCreate(int dimension);
void freeMatrix(Matrix* m);
void fillMatrix(Matrix* m, City* from, City* dest, int distance);


#endif //MATRIX_H