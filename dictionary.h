#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "city.h"

typedef struct dictionnaire Dico;

// Constructeur :
void addCity(Dico* dico, City* city);
Dico* createDictionary();

//Opérateurs :
void removeCity(Dico* dico, City* city);
void getSize(Dico* dico);

// Getters
int getDist(char* city);
#endif //DICTIONARY_H

// Nous avons crée le type Dico afin de pouvoir potentiellement l'utiliser plus tard
// Fort potentiel de fusion avec le fichier Ville.