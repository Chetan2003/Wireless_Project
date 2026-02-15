#include <stdlib.h>
#include <math.h> // For sqrt
#include "wireless.h"

SignalStream modulator_qpsk(BitStream input) {
    SignalStream output;

    // QPSK packs 2 bits into 1 symbol
    // Length is half of the bitstream length
    output.length = input.length / 2;
    output.symbols = (Complex*)malloc(output.length * sizeof(Complex));

    // Normalization Factor: 1/sqrt(2) = 0.707106
    // This ensures Symbol Energy (Es) = 1.0
    double scale = 0.70710678;

    for (int i = 0; i < output.length; i++) {
        // Take two bits: input[2*i] and input[2*i + 1]
        int bit_I = input.bits[2 * i];     // Even bit -> I channel
        int bit_Q = input.bits[2 * i + 1]; // Odd bit  -> Q channel

        // Map 0 -> -0.707, 1 -> +0.707
        output.symbols[i].I = (bit_I == 1) ? scale : -scale;
        output.symbols[i].Q = (bit_Q == 1) ? scale : -scale;
    }

    return output;
}