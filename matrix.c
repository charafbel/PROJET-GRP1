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
    }

    for (int i = 0; i < dimension; i++) {
        mat[i] = malloc(sizeof(int) * (dimension - i - 1));  // ✅ Fix
        for (int j = 0; j < (dimension - i - 1); j++) {
            mat[i][j] = 0;
            printf("matrice [%d][%d] = %d\n", i, j, mat[i][j]);
        }
    }


    Matrix* m = malloc(sizeof(Matrix));
    if (!m) {
        perror("malloc Matrix");
        exit(EXIT_FAILURE);
    }

    m->matrix = mat;
    m->dimension = dimension;
    printf("[DEBUG] Matrice créée avec succès\n");
    return m;
}

void freeMatrix(Matrix *m) {
    for (int i = 0; i < m->dimension; i++) {
        free(m->matrix[i]);
    }
    free(m->matrix);
    free(m);
}

/* Dans matrix.c */
// ... (après freeMatrix)

/**
 * @brief Définit la distance entre les villes i et j.
 * Gère la symétrie et le mapping d'index.
 */
void setDistance(Matrix* m, int i, int j, int distance) {
    // 1. Gérer la symétrie : on s'assure que i <= j
    if (i > j) {
        int temp = i;
        i = j;
        j = temp;
    }

    // 2. Vérifier les bornes (simple vérification)
    if (i >= m->dimension || j >= m->dimension) {
        fprintf(stderr, "Erreur setDistance: Index hors bornes (%d, %d)\n", i, j);
        return;
    }

    // 3. Appliquer la formule de mapping
    m->matrix[i][j - i] = distance;
}

/**
 * @brief Récupère la distance entre les villes i et j.
 * Gère la symétrie et le mapping d'index.
 */
int getDistance(Matrix* m, int i, int j) {
    // 1. Gérer la symétrie : on s'assure que i <= j
    if (i > j) {
        int temp = i;
        i = j;
        j = temp;
    }

    // 2. Vérifier les bornes
    if (i >= m->dimension || j >= m->dimension) {
        fprintf(stderr, "Erreur getDistance: Index hors bornes (%d, %d)\n", i, j);
        return -1; // Ou une autre valeur d'erreur
    }

    // 3. Appliquer la formule de mapping
    return m->matrix[i][j - i];
}

void fillMatrix(Matrix* m, City* from, City* dest, int distance) {
    int i = from->id;
    int j = dest->id;
    if (i < j)
        m->matrix[i][j] = distance;
}