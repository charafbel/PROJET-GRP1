#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "city.h"

typedef struct Dictionary Dic;

// Constructeurs :
Dic* createDictionary();
void freeDictionary(Dic* dictionary);
Dic* addToDictionary(Dic* dictionary, City* city);

// Opérateurs :
Dic* removeFromDictionary(Dic* dictionary);

// Getters :
int getDistance(City* city1, City* city2);
City* getCity(char* name);
int getSize(Dic* dictionary);
#endif //DICTIONARY_H

// Nous avons crée le type Dico afin de pouvoir potentiellement l'utiliser plus tard
// Fort potentiel de fusion avec le fichier Ville.