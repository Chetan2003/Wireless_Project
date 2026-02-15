#include <stdlib.h>
#include "wireless.h"

BitStream source_encoder(char* text, long text_len) {
    BitStream stream;
    stream.length = text_len * BITS_PER_CHAR;
    stream.bits = (int*)malloc(stream.length * sizeof(int));

    int bit_idx = 0;
    for (int i = 0; i < text_len; i++) {
        unsigned char c = text[i];
        for (int b = BITS_PER_CHAR - 1; b >= 0; b--) {
            stream.bits[bit_idx++] = (c >> b) & 1;
        }
    }
    return stream;
}