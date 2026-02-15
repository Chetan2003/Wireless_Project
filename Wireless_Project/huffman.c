#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wireless.h"

#define MAX_TREE_HT 100

// --- HELPER FUNCTIONS ---
HuffmanNode* newNode(char data, unsigned freq) {
    HuffmanNode* temp = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (!temp) exit(1);
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

typedef struct {
    unsigned size;
    unsigned capacity;
    HuffmanNode** array;
} MinHeap;

MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!minHeap) exit(1);
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode**)malloc(minHeap->capacity * sizeof(HuffmanNode*));
    if (!minHeap->array) exit(1);
    return minHeap;
}

void swapMinHeapNode(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* t = *a; *a = *b; *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < (int)minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < (int)minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

HuffmanNode* extractMin(MinHeap* minHeap) {
    if (minHeap->size == 0) return NULL;
    HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, HuffmanNode* minHeapNode) {
    if (!minHeapNode) return;
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

int isSizeOne(MinHeap* minHeap) { return (minHeap->size == 1); }

HuffmanNode* build_huffman_tree(char* text, long len) {
    unsigned freq[256] = { 0 };
    for (long i = 0; i < len; i++) freq[(unsigned char)text[i]]++;

    MinHeap* minHeap = createMinHeap(256);
    for (int i = 0; i < 256; ++i)
        if (freq[i] > 0) insertMinHeap(minHeap, newNode((char)i, freq[i]));

    while (!isSizeOne(minHeap)) {
        HuffmanNode* left = extractMin(minHeap);
        HuffmanNode* right = extractMin(minHeap);
        HuffmanNode* top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    HuffmanNode* root = extractMin(minHeap);
    free(minHeap->array);
    free(minHeap);
    return root;
}

void storeCodes(HuffmanNode* root, int arr[], int top, char (*codes)[MAX_TREE_HT], int codeLen[]) {
    if (!root) return;
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes, codeLen);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes, codeLen);
    }
    if (!root->left && !root->right) {
        for (int i = 0; i < top; i++) codes[(unsigned char)root->data][i] = arr[i];
        codeLen[(unsigned char)root->data] = top;
    }
}

BitStream huffman_encoder(char* text, long len, HuffmanNode* root) {
    char (*codes)[MAX_TREE_HT] = (char (*)[MAX_TREE_HT])malloc(256 * MAX_TREE_HT);
    int* codeLen = (int*)calloc(256, sizeof(int));
    int* arr = (int*)malloc(MAX_TREE_HT * sizeof(int));

    if (!codes || !codeLen || !arr) exit(1);

    int top = 0;
    storeCodes(root, arr, top, codes, codeLen);

    long total_bits = 0;
    for (long i = 0; i < len; i++) total_bits += codeLen[(unsigned char)text[i]];

    BitStream output;
    output.length = total_bits;
    output.bits = (int*)malloc(total_bits * sizeof(int));
    if (!output.bits) exit(1);

    long bit_idx = 0;
    for (long i = 0; i < len; i++) {
        unsigned char c = text[i];
        for (int j = 0; j < codeLen[c]; j++) {
            // FIX FOR C6386: Explicitly check bounds
            if (bit_idx < total_bits) {
                output.bits[bit_idx++] = codes[c][j];
            }
        }
    }

    free(codes); free(codeLen); free(arr);
    return output;
}

char* huffman_decoder(BitStream input, HuffmanNode* root) {
    char* output_text = (char*)malloc(input.length + 1);
    if (!output_text) exit(1);

    int char_idx = 0;
    HuffmanNode* curr = root;

    for (int i = 0; i < input.length; i++) {
        if (!curr) break;
        if (input.bits[i] == 0) curr = curr->left;
        else curr = curr->right;

        if (curr && !curr->left && !curr->right) {
            output_text[char_idx++] = curr->data;
            curr = root;
        }
    }
    output_text[char_idx] = '\0';
    return output_text;
}

void free_huffman_tree(HuffmanNode* root) {
    if (!root) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}