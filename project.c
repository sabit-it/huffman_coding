#include <stdio.h>
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
    return -1;
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


struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp =
            (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap =
            (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
            (struct MinHeapNode**) malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size &&
        minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size &&
        minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1)/2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1)/2];
        i = (i - 1)/2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printArr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right) ;
}


char* arrayToString(int array[], int length) {
    char *result = (char*) malloc(sizeof(char) * 1024);
    int i, offset = 0;

    for (i = 0; i < length; i++) {
        offset += sprintf(result + offset, "%d", array[i]);
    }

    return result;
}

char* arrayToStringSplit(int array[], int l, int r){
    char *result = (char*) malloc(sizeof(char) * 1024);
    int i, offset = 0;

    for (i = l; i < r; i++) {
        offset += sprintf(result + offset, "%d", array[i]);
    }

    return result;
}


char* string[256];
int str[50];
int i = 0;
Map codes;
int is_first = 0;


void coding(struct MinHeapNode* cur){
    if(is_first == 0){
        is_first = 1;
        MapInit(&codes, 256);
    }
    if(cur->left != NULL){
        str[i] = 0;
        ++i;
        coding(cur->left);
    }
    if(cur->right != NULL){
        str[i] = 1;
        ++i;
        coding(cur->right);
    }
    if(isLeaf(cur)) {
        char *temp = arrayToString(str, i);
        MapSet(&codes, temp, cur->data);
        string[cur->data] = temp;
        printf("%s %c\n", string[cur->data], cur->data);
    }
    --i;
}


void decoding(){
    FILE *input, *output;
    char ch;
    input = fopen("output.txt", "r");
    output = fopen("output_decoding.txt", "w");
    int l = 0;
    int r = 0;
    int digits[256];
    while((ch = fgetc(input)) != EOF){
        if(ch == '0'){
            digits[r] = 0;
        } else {
            digits[r] = 1;
        }
        r++;
        char *temp = arrayToStringSplit(digits, l, r);
        if(MapGet(&codes, temp) != -1){
            fprintf(output, "%c", MapGet(&codes, temp));
            l = r;
        }
    }
    fclose(input);
    fclose(output);
}


int main() {
    FILE *input, *output;
    char ch;

    int arr[256];
    for(int i = 0; i < 256; i++){
        arr[i] = 0;
    }

    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    if (input == NULL) {
        printf("Не удалось открыть файл.");
        return 1;
    }

    while ((ch = fgetc(input)) != EOF) {
        arr[ch]++;
    }
    struct MinHeap* heap = createMinHeap(256);
    for(int i = 0; i < 256; i++){
        if(arr[i] == 0) continue;
        struct MinHeapNode* temp = newNode(i, arr[i]);
        insertMinHeap(heap, temp);
    }
    buildMinHeap(heap);

    while (heap->size > 1){
        struct MinHeapNode* temp1 = extractMin(heap);
        struct MinHeapNode* temp2 = extractMin(heap);
        struct MinHeapNode* root = newNode(-1, temp1->freq + temp2->freq);
        root->left = temp1;
        root->right = temp2;
        insertMinHeap(heap, root);

    }
    struct MinHeapNode* root = extractMin(heap);
    printf("%d\n", root->freq);

    coding(root);
    fclose(input);
    input = fopen("input.txt", "r");
    while ((ch = fgetc(input)) != EOF) {
        fprintf(output, "%s", string[ch]);
    }
    fclose(input);
    fclose(output);
    decoding();
    MapFree(&codes);
    return 0;
}

