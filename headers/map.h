#pragma once

typedef struct {
    char *key;
    int value;
} KeyValuePair;

typedef struct {
    KeyValuePair *data;
    int size;
} Map;


void initMap(Map *map, int size);

void freeMap(Map *map);

int getMap(Map *map, const char *key);

void setMap(Map *map, const char *key, int value);

