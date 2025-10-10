#ifndef DICTIONARY_H
#define DICTIONARY_H

typedef struct dictionnaire Dico;

int getDist(char* city);
City* getCity(int val);

#endif //DICTIONARY_H

// Nous avons crée le type Dico afin de pouvoir potentiellement l'utiliser plus tard
// Fort potentiel de fusion avec le fichier Ville.