#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONSOLE_INFO
#define SIZE 100000

typedef struct {
    char *key;
    int value;
} KeyValuePair;

typedef struct {
    KeyValuePair *data;
    int size;
} Map;


void initMap(Map *map, int size) {
    map->data = (KeyValuePair *) malloc(sizeof(KeyValuePair) * size);
    map->size = size;
    memset(map->data, 0, sizeof(KeyValuePair) * size);
}

void freeMap(Map *map) {
    for (int i = 0; i < map->size; i++) {
        if (map->data[i].key != NULL) {
            free(map->data[i].key);
        }
    }
    free(map->data);
}

int getMap(Map *map, const char *key) {
    for (int i = 0; i < map->size; i++) {
        if (map->data[i].key != NULL && strcmp(map->data[i].key, key) == 0) {
            return map->data[i].value;
        }
    }
    return -1;
}

void setMap(Map *map, const char *key, int value) {
    for (int i = 0; i < map->size; i++) {
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


void coding(Map *codes, struct MinHeapNode* cur, char* string[], int str[], int* i, int is_first){
    if(is_first == 0){
        initMap(codes, SIZE);
    }

    if(cur->left != NULL){
        str[*i] = 0;
        ++(*i);
        coding(codes, cur->left, string, str, i, 1);
    }

    if(cur->right != NULL){
        str[*i] = 1;
        ++(*i);
        coding(codes, cur->right, string, str, i, 1);
    }

    if(isLeaf(cur)) {
        char *temp = arrayToString(str, *i);
        setMap(codes, temp, cur->data);
        string[cur->data] = temp;
#ifdef CONSOLE_INFO
        printf("%s %c\n", string[cur->data], cur->data);
#endif
    }
    --(*i);
}


void decoding(Map *codes){
    FILE *input, *output;
    char ch;
    input = fopen("output.txt", "r");
    output = fopen("output_decoding.txt", "w");
    int l = 0;
    int r = 0;
    int digits[SIZE];
    while((ch = fgetc(input)) != EOF){
        if(ch == '0'){
            digits[r] = 0;
        } else {
            digits[r] = 1;
        }
        r++;
        char *temp = arrayToStringSplit(digits, l, r);
        if(getMap(codes, temp) != -1){
            fprintf(output, "%c", getMap(codes, temp));
            l = r;
        }
    }
    fclose(input);
    fclose(output);
}


struct MinHeapNode* buildHuffmanTree(){
    FILE *input, *output;
    char ch;

    int arr[SIZE];
    for(int i = 0; i < SIZE; i++){
        arr[i] = 0;
    }

    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    while ((ch = fgetc(input)) != EOF) {
        arr[ch]++;
    }
    struct MinHeap* heap = createMinHeap(SIZE);
    for(int i = 0; i < SIZE; i++){
        if(arr[i] == 0) continue;
        struct MinHeapNode* temp = newNode(i, arr[i]);
        insertMinHeap(heap, temp);
    }
    buildMinHeap(heap);

    while (!isSizeOne(heap)){
        struct MinHeapNode* temp1 = extractMin(heap);
        struct MinHeapNode* temp2 = extractMin(heap);
        struct MinHeapNode* root = newNode(-1, temp1->freq + temp2->freq);
        root->left = temp1;
        root->right = temp2;
        insertMinHeap(heap, root);

    }
    struct MinHeapNode* root = extractMin(heap);
    fclose(input);
    fclose(output);
    return root;
}

void printCodedText(char* string[]){
    FILE *input, *output;
    char ch;
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    while ((ch = fgetc(input)) != EOF) {
        fprintf(output, "%s", string[ch]);
    }
    fclose(input);
    fclose(output);
}


void printData(){
    FILE *input, *output, *info;
    long input_size, coded_size;

    input = fopen("input.txt", "rb");
    fseek(input, 0, SEEK_END);
    input_size = ftell(input);
    fclose(input);

    output = fopen("output.txt", "rb");
    fseek(output, 0, SEEK_END);
    coded_size = ftell(output);
    fclose(output);

    info = fopen("info.txt", "w");
    fprintf(info, "input file size: %ld bytes\n", input_size);
    fprintf(info, "coded file size: %ld bytes\n", coded_size / 8);
    fclose(info);
}

int main() {
    char* string[SIZE];
    int str[50];
    int i = 0;
    Map codes;
    initMap(&codes, SIZE);

    coding(&codes, buildHuffmanTree(), string, str, &i, 0);
    printCodedText(string);
    decoding(&codes);
    printData();
    freeMap(&codes);
    return 0;
}
