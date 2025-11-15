#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <signal.h>

#include "tad/matrix.h"
#include "tad/tsp.h"

#ifndef BRUTFORCE_H
#define BRUTFORCE_H

void swapArrVal(int *a, int *b);
bool nextPermutation(int *arr, int n);
int totalPathDistance(Matrix *m, int *path, int n);
Results* brutForce(Matrix *m);

#endif
