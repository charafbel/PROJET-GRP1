#include "dictionary.h"

typedef struct couple{
    int ID;
    City* city;
} couple;

struct dictionary{
    couple[] dic;
    int size;
}

