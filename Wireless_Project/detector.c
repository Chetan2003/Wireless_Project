#include <stdlib.h>
#include "wireless.h"

BitStream detector(SignalStream input) {
    BitStream output;

    // Output bits = 2 * Input Symbols
    output.length = input.length * 2;
    output.bits = (int*)malloc(output.length * sizeof(int));

    for (int i = 0; i < input.length; i++) {
        // Decision Rule: Check the Sign (Zero Threshold)

        // 1. Decode I Component (Even Bits)
        if (input.symbols[i].I >= 0.0) {
            output.bits[2 * i] = 1;
        }
        else {
            output.bits[2 * i] = 0;
        }

        // 2. Decode Q Component (Odd Bits)
        if (input.symbols[i].Q >= 0.0) {
            output.bits[2 * i + 1] = 1;
        }
        else {
            output.bits[2 * i + 1] = 0;
        }
    }
    return output;
}