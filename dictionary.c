#include "dictionary.h"
#include "city.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct couple{
    int ID;
    City* city;
} couple;

struct dictionary{
    couple dic[];
    int size;
};

Dic* createDictionary() {
    Dic* dictionary = malloc(sizeof(dictionary));
    dictionary->size = 0;
    return dictionary;
}
void freeDictionary(Dic* dictionary) {
    free(dictionary);
}
Dic* addToDictionary(Dic* dictionary, City* city) {
    couple* couple = malloc(sizeof(couple));
    couple->city = city;
    couple->ID = dictionary->size;
    (dictionary->size)++;
}

Dic* removeFromDictionary(Dic* dictionary, City* city) {
    int delIndex;
    for (int i = 0; i < dictionary->size; i++) {
        if (strcmp(((dictionary->dic[i])->city)->name, city->name)) {
            delIndex = i;
        }
    }
    for (int i = delIndex; i < dictionary->size; i++) {
        dictionary->dic[i] = dictionary->dic[i + 1];
    }
    dictionary = realloc(sizeof(dictionary));
    return dictionary;
}
int getDistance(City* city1, City* city2) {
    //TODO
}
City* getCity(char* name) {
    for (int i = 0; i < dictionary->size; i++) {
        if (strcmp(((dictionary->dic[i])->city)->name, name)) {
            return (dictionary->dic[i])->city;
        }
    }
    return NULL;
}
int getSize(Dic* dictionary) {
    return dictionary->size;
}