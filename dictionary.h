#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "city.h"

typedef struct {
    int dimension;
    int **matrix;
} Matrix;

Matrix* MatrixCreate(int dimension);
void freeMatrix(Matrix* m);



#endif //DICTIONARY_H