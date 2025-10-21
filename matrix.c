#include "matrix.h"
#include "city.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Matrix* MatrixCreate(int dimension) {
    int **mat = malloc(sizeof(int*) * dimension);
    if (!mat) {
        perror("malloc mat");
        exit(EXIT_FAILURE);
    for (int i = 0; i < dimension; i++) {
        // Ligne i contient les colonnes j >= i
        mat[i] = malloc(sizeof(int) * (dimension - i));
        if (!mat[i]) {
            fprintf(stderr, "[ERREUR] malloc ligne %d échoué\n", i);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < (dimension - i); j++)
            mat[i][j] = 0;
        printf("[DEBUG] Ligne %d allouée : %d éléments (colonnes %d → %d)\n", i+1, dimension - i, i, dimension - 1);
    }
    Matrix* m = malloc(sizeof(Matrix));
    if (!m) {
        perror("malloc Matrix");
        exit(EXIT_FAILURE);
    }
    m->matrix = mat;
    m->dimension = dimension;
    printf("[DEBUG] Matrice créée avec succès");
    return m;
}

void freeMatrix(Matrix *m) {
    for (int i = 0; i < m->dimension; i++) {
        free(m->matrix[i]);
    }
    free(m->matrix);
    free(m);
}
void fillMatrix(Matrix* m, City* from, City* dest, int distance) {
    if (m == NULL || m->matrix == NULL) {
        fprintf(stderr, "Error fillMatrix()\n");
        exit(EXIT_FAILURE);
    }

    int i = from->id;
    int j = dest->id;
    if (i < j)
        m->matrix[i][j] = distance;
}



