#include <stdlib.h>
#include <string.h>
#include "map.h"

void MapInit(Map *map, int size) {
    map->data = (KeyValuePair *) malloc(sizeof(KeyValuePair) * size);
    map->size = size;
    memset(map->data, 0, sizeof(KeyValuePair) * size);
}

void MapFree(Map *map) {
    int i;
    for (i = 0; i < map->size; i++) {
        if (map->data[i].key != NULL) {
            free(map->data[i].key);
        }
    }
    free(map->data);
}

int MapGet(Map *map, const char *key) {
    int i;
    for (i = 0; i < map->size; i++) {
        if (map->data[i].key != NULL && strcmp(map->data[i].key, key) == 0) {
            return map->data[i].value;
        }
    }
    return 0;
}

void MapSet(Map *map, const char *key, int value) {
    int i;
    for (i = 0; i < map->size; i++) {
        if (map->data[i].key == NULL) {
            map->data[i].key = (char *) malloc(strlen(key) + 1);
            strcpy(map->data[i].key, key);
            map->data[i].value = value;
            return;
        }
        else if (strcmp(map->data[i].key, key) == 0) {
            map->data[i].value = value;
            return;
        }
    }
}
