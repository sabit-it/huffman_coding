#pragma once

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

struct MinHeapNode* newNode(char data, unsigned freq);

struct MinHeap* createMinHeap(unsigned capacity);

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);

void minHeapify(struct MinHeap* minHeap, int idx);

int isSizeOne(struct MinHeap* minHeap);

struct MinHeapNode* extractMin(struct MinHeap* minHeap);

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode);

void buildMinHeap(struct MinHeap* minHeap);

int isLeaf(struct MinHeapNode* root);