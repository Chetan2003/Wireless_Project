#include <stdlib.h>
#include "wireless.h"

BitStream channel_encoder(BitStream input) {
    BitStream output;
    output.length = input.length * REPETITION_FACTOR;
    output.bits = (int*)malloc(output.length * sizeof(int));

    int idx = 0;
    for (int i = 0; i < input.length; i++) {
        for (int r = 0; r < REPETITION_FACTOR; r++) {
            output.bits[idx++] = input.bits[i];
        }
    }
    return output;
}