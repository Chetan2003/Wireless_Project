#include <stdlib.h>
#include "wireless.h"

BitStream channel_decoder(BitStream input) {
    BitStream output;
    output.length = input.length / REPETITION_FACTOR;
    output.bits = (int*)malloc(output.length * sizeof(int));

    for (int i = 0; i < output.length; i++) {
        int ones = 0;
        for (int r = 0; r < REPETITION_FACTOR; r++) {
            if (input.bits[i * REPETITION_FACTOR + r] == 1) ones++;
        }
        output.bits[i] = (ones > REPETITION_FACTOR / 2) ? 1 : 0;
    }
    return output;
}