#ifndef MATRIX_H
#define MATRIX_H

#include "city.h"

typedef struct {
    int dimension;
    int **matrix;
} Matrix;

typedef struct {
    int dimension;
    int bestDistance;
    int *bestPath;
} Results;


Matrix* MatrixCreate(int dimension);
void freeMatrix(Matrix* m);
void setDistance(Matrix* m, int i, int j, int distance);
int getDistance(Matrix* m, int i, int j);

#endif //MATRIX_H