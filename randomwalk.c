#include <stdlib.h>
#include <time.h>
#include "tad/city.h"
#include "tad/matrix.h"
#include "tad/tsp.h"
#include "randomwalk.h"

/* Determine longueur cycle fermé qui est défini par chemin[0/n-1]  */
static int totalPathDistance(Matrix *m,const int *chemin,  int n) {
     int somme = 0;
     for (int i = 0; i < n - 1; i++) {
    somme +=getDistance(m,chemin[i],   chemin[i + 1]);
                    }
    somme += getDistance(m,chemin[n-1], chemin[0]); /* retour au départ */
     return somme;}

static void shuffle(  int *arr,     int n){
     for (int i = n-1;   i > 0;--i) {
          int j = rand() % (i + 1);
        int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;}}

Results* randomWalk(Matrix* m) {
    if (!m || m->dimension <= 0)   return NULL;

    /*  RNG une seule fois par processus */
           static int x = 0;
     if (!x) { srand((unsigned int)time(NULL)); x = 1; }

    int n = m->dimension;

        /* construire une permutation 0..n-1 puis mélanger */
      int *perm = (int*) malloc(sizeof(int) * n);
    if (!perm) { perror("malloc randomWalk perm"); exit(EXIT_FAILURE); }
     for (int i = 0; i < n; ++i) perm[i] = i;
    shuffle(perm, n);

     /* distance de la tournée */
    int dist  = totalPathDistance(m, perm, n);

    /* empaqueter dans Results (mêmes conventions que le reste du projet) */
      Results *result = (Results*)malloc(sizeof(Results));
    if(!result)   { perror("malloc mauvais randomWalk resultat"); exit(EXIT_FAILURE); }
        result->bestDistance = dist;

     result->dimension = n;
    result->bestPath = (int*)malloc(sizeof(int) * n);
     if (!result->bestPath) { perror("malloc mauvais  randomWalk chemin");    exit(EXIT_FAILURE); }
       for (int i = 0; i < n; ++i)    {result->bestPath[i] =perm[i];}

      free(perm);
     return result;}
