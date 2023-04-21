#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers//map.h"
#include "headers//min_heap.h"

#define CONSOLE_INFO
#define SIZE 100000

void printArr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
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


void coding(Map *codes, struct MinHeapNode* cur, char* string[], int str[], int* i, int is_first, FILE* map_output){
    if(is_first == 0){
        initMap(codes, SIZE);
    }

    if(cur->left != NULL){
        str[*i] = 0;
        ++(*i);
        coding(codes, cur->left, string, str, i, 1, map_output);
    }

    if(cur->right != NULL){
        str[*i] = 1;
        ++(*i);
        coding(codes, cur->right, string, str, i, 1, map_output);
    }

    if(isLeaf(cur)) {
        char *temp = arrayToString(str, *i);
        setMap(codes, temp, cur->data);
        string[cur->data] = temp;
        fprintf(map_output, "%s %c\n", string[cur->data], cur->data);
#ifdef CONSOLE_INFO
        printf("%s %c\n", string[cur->data], cur->data);
#endif
    }
    --(*i);
}


void decoding(Map *codes){
    FILE *input, *output;
    char ch;
    input = fopen("txt_files/output.txt", "r");
    output = fopen("txt_files/output_decoding.txt", "w");
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

    input = fopen("txt_files/input.txt", "r");
    output = fopen("txt_files/output.txt", "w");

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
    input = fopen("txt_files/input.txt", "r");
    output = fopen("txt_files/output.txt", "w");
    while ((ch = fgetc(input)) != EOF) {
        fprintf(output, "%s", string[ch]);
    }
    fclose(input);
    fclose(output);
}


void printData(){
    FILE *input, *output, *info;
    long input_size, coded_size;

    input = fopen("txt_files/input.txt", "rb");
    fseek(input, 0, SEEK_END);
    input_size = ftell(input);
    fclose(input);

    output = fopen("txt_files/output.txt", "rb");
    fseek(output, 0, SEEK_END);
    coded_size = ftell(output);
    fclose(output);

    info = fopen("txt_files/info.txt", "w");
    fprintf(info, "input file size: %ld bytes\n", input_size);
    fprintf(info, "coded file size: %ld bytes\n", coded_size / 8);
    fclose(info);
}

int main() {
    FILE* map_output;
    map_output = fopen("map_output.txt", "w");
    char* string[SIZE];
    int str[50];
    int i = 0;
    Map codes;
    initMap(&codes, SIZE);

    coding(&codes, buildHuffmanTree(), string, str, &i, 0, map_output);
    printCodedText(string);
    decoding(&codes);
    printData();
    freeMap(&codes);
    fclose(map_output);
    return 0;
}
