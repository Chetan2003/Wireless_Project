#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "wireless.h"

// --- LINE ENCODER (Differential NRZ-I) ---
// 1 = Toggle state, 0 = Keep state
BitStream line_encoder_differential(BitStream input) {
    BitStream output;
    output.length = input.length;
    output.bits = (int*)malloc(output.length * sizeof(int));
    if (!output.bits) {
        printf("Memory allocation failed (Line Encoder)\n");
        exit(1);
    }

    int current_state = 0; // Starting reference state

    for (int i = 0; i < input.length; i++) {
        if (input.bits[i] == 1) {
            // Toggle the state (0 becomes 1, 1 becomes 0)
            current_state = 1 - current_state;
        }
        output.bits[i] = current_state;
    }
    return output;
}

// --- LINE DECODER (Differential NRZ-I) ---
// Change = 1, No Change = 0
BitStream line_decoder_differential(BitStream input) {
    BitStream output;
    output.length = input.length;
    output.bits = (int*)malloc(output.length * sizeof(int));
    if (!output.bits) {
        printf("Memory allocation failed (Line Decoder)\n");
        exit(1);
    }

    int prev_state = 0; // Must match the encoder's starting reference

    for (int i = 0; i < input.length; i++) {
        // If the current bit is different from the previous bit, it means a 1 was sent
        if (input.bits[i] != prev_state) {
            output.bits[i] = 1;
        }
        else {
            // If it stayed the same, a 0 was sent
            output.bits[i] = 0;
        }
        // Update the reference for the next bit
        prev_state = input.bits[i];
    }
    return output;
}