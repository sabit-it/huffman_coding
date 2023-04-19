#pragma once
typedef struct {
    char *key;
    int value;
} KeyValuePair;

typedef struct {
    KeyValuePair *data;
    int size;
} Map;

void MapInit(Map *map, int size);
void MapFree(Map *map);
int MapGet(Map *map, const char *key);
void MapSet(Map *map, const char *key, int value);
