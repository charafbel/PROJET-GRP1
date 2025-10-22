#include <stdlib.h>
#include <string.h>

#include "tad/city.h"
#include "tad/matrix.h"
#include "tad/tsp.h"

void brutForce(Matrix* m) {

}

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <tadfile> <nameFile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /* LECTURE DU FICHIER */
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "Cant open the file.\n");
        exit(EXIT_FAILURE);
    }

    Infos* infos = readTsp(f);

    /* Choix du type de fonction */
    int (*fctd)(City*, City*) = NULL;
    if (strcmp(infos->edgeType, "ATT") == 0)
        fctd = distanceAtt;
    else if (strcmp(infos->edgeType, "EUCL_2D") == 0)
        fctd = distanceEucl; // A CHANGER
    else if (strcmp(infos->edgeType, "GEO") == 0)
        fctd = distanceAtt; // A CHANGER
    // Gestion du cas d'erreur.
    if (fctd == NULL) {
        fprintf(stderr, "Edgetype unknown\n");
        // Liberation Memoire
        free(infos->cityArray);
        free(infos);
        exit(EXIT_FAILURE);
    }

    Matrix* m = distanceMatrix(*infos, fctd);
    printMatrix(m);

    brutforce(m);

    freeMatrix(m);
    return 0;
}