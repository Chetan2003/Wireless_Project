#include <stdlib.h>
#include "wireless.h"

char* source_decoder(BitStream input) {
    int char_count = input.length / BITS_PER_CHAR;
    char* text = (char*)malloc((char_count + 1) * sizeof(char));

    for (int i = 0; i < char_count; i++) {
        unsigned char c = 0;
        for (int b = 0; b < BITS_PER_CHAR; b++) {
            c = (c << 1) | input.bits[i * BITS_PER_CHAR + b];
        }
        text[i] = c;
    }
    text[char_count] = '\0';
    return text;
}