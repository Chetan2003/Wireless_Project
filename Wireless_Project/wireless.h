#ifndef WIRELESS_H
#define WIRELESS_H

// --- CONFIGURATION ---
#define REPETITION_FACTOR 3  
#define BITS_PER_CHAR 8      

// --- DATA STRUCTURES ---
typedef struct {
    int* bits;
    int length;
} BitStream;

// Complex Number for QPSK
typedef struct {
    double I;
    double Q;
} Complex;

typedef struct {
    Complex* symbols;
    int length;
} SignalStream;

// Huffman Tree Node
typedef struct HuffmanNode {
    char data;
    unsigned freq;
    struct HuffmanNode* left, * right;
} HuffmanNode;

// --- FUNCTION PROTOTYPES ---

// Standard Blocks
BitStream channel_encoder(BitStream input);
SignalStream modulator_qpsk(BitStream input); // Ensure this is QPSK
SignalStream awgn_channel(SignalStream input, double snr_db);
BitStream detector(SignalStream input);
BitStream channel_decoder(BitStream input);

// Huffman Functions
HuffmanNode* build_huffman_tree(char* text, long len);
BitStream huffman_encoder(char* text, long len, HuffmanNode* root);
char* huffman_decoder(BitStream input, HuffmanNode* root);
void free_huffman_tree(HuffmanNode* root);

#endif