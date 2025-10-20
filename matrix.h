#ifndef MATRIX_H
#define MATRIX_H

#include "city.h"

typedef struct {
    int dimension;
    int **matrix;
} Matrix;

Matrix* MatrixCreate(int dimension);
void freeMatrix(Matrix* m);



#endif //MATRIX_H