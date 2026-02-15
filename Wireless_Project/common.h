#ifndef COMMON_H
#define COMMON_H

// --- CONFIGURATION ---
#define REPETITION_FACTOR 3  
#define BITS_PER_CHAR 8      

// --- DATA STRUCTURES ---
typedef struct {
    int* bits;
    int length;
} BitStream;

// NEW: Complex Number Structure for QPSK
typedef struct {
    double I; // In-Phase (Real part)
    double Q; // Quadrature (Imaginary part)
} Complex;

typedef struct {
    Complex* symbols; // Array of Complex numbers
    int length;       // Number of symbols
} SignalStream;

typedef struct HuffmanNode {
    char data;
    unsigned freq;
    struct HuffmanNode* left, * right;
} HuffmanNode;

#endif