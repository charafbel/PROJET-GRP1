#include "dictionary.h"
#include "city.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Dic* createDictionary() {
    Dic* dictionary = malloc(sizeof(Dic));
    dictionary->size = 0;
    return dictionary;
}

void freeDictionary(Dic* dictionary) {
    free(dictionary);
}

Dic* addToDictionary(Dic* dictionary, City* city) {
    // Redimensionner le tableau dic pour accueillir un nouvel élément
    Couple* temp = realloc(dictionary->dic, (dictionary->size + 1) * sizeof(Couple));
    if (!temp) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    (dictionary->couple) = temp;

    // Remplir la nouvelle case
    (dictionary->couple).city = city;
    (dictionary->couple).ID = city->id;

    dictionary->size++;

    return dictionary;
}


Dic* removeFromDictionary(Dic* dictionary, City* city) {
    int delIndex;
    for (int i = 0; i < dictionary->size; i++) {
        if ((dictionary->dic[i]).city->id == city->id) {
            delIndex = i;
        }
    }
    for (int i = delIndex; i < dictionary->size; i++) {
        dictionary->dic[i] = dictionary->dic[i + 1];
    }
    return dictionary;
}

/*
int getDistance(City* city1, City* city2) {
    //TODO
}

int getSize(Dic* dictionary) {
    return dictionary->size;
}
*/