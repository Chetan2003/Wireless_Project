#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wireless.h"

void save_colored_output(char* original, char* decoded, double snr) {
    char filename[50];
    sprintf(filename, "output_%.0fdB.html", snr);
    FILE* f = fopen(filename, "w");
    if (!f) return;

    fprintf(f, "<html><head><style>body{font-family:monospace; white-space:pre-wrap;}</style></head><body>");
    fprintf(f, "<h2>Received Text at SNR = %.1f dB</h2><hr>", snr);

    size_t len = strlen(original);
    for (size_t i = 0; i < len; i++) {
        if (original[i] == decoded[i]) {
            fputc(decoded[i], f);
        }
        else {
            fprintf(f, "<span style='color:red; font-weight:bold; background-color:#ffdddd;'>%c</span>", decoded[i]);
        }
    }
    fprintf(f, "</body></html>");
    fclose(f);
    printf("   -> Saved visualization to '%s'\n", filename);
}

int main() {
    srand((unsigned int)time(NULL));

    const char* filename = "Text Source.txt";
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Error: Could not open '%s'.\n", filename);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize <= 0) {
        printf("Error: File is empty.\n");
        fclose(f);
        return 1;
    }

    char* original_text = (char*)malloc(fsize + 1);
    if (!original_text) {
        printf("Error: Memory allocation failed.\n");
        fclose(f);
        return 1;
    }

    fread(original_text, 1, fsize, f);
    fclose(f);
    original_text[fsize] = 0;

    printf("Input Loaded: %ld chars.\n", fsize);

    printf("Building Huffman Tree...\n");
    HuffmanNode* huff_root = build_huffman_tree(original_text, fsize);

    printf("Huffman Tree Built.\n");
    printf("------------------------------------------------------------\n");
    printf("|  SNR (dB)  |  Total Bits  |  Bit Errors  |      SER      |\n");
    printf("------------------------------------------------------------\n");

    double snr_values[] = { -5.0, 0.0, 5.0, 10.0, 15.0 };
    int num_snrs = 5;

    for (int s = 0; s < num_snrs; s++) {
        double current_snr = snr_values[s];

        // 1. Huffman Encoder
        BitStream bits_src = huffman_encoder(original_text, fsize, huff_root);

        // 2. Channel Encoder
        BitStream bits_enc = channel_encoder(bits_src);

        //3. Line Encoder (Differential NRZ-I) ---
        BitStream bits_line = line_encoder_differential(bits_enc);

        // 4. Modulator (QPSK)
        SignalStream sig_tx = modulator_qpsk(bits_line);

        // 5. Channel
        SignalStream sig_rx = awgn_channel(sig_tx, current_snr);

        // 6. Detector
        BitStream bits_det = detector(sig_rx);

        //7. Line Decoder (Differential NRZ-I) ---
        BitStream bits_line_dec = line_decoder_differential(bits_det);

        // 8. Channel Decoder
        BitStream bits_dec = channel_decoder(bits_line_dec);

        // 9. Huffman Decoder
        char* final_text = huffman_decoder(bits_dec, huff_root);

        // Errors
        int errors = 0;
        for (int i = 0; i < bits_src.length; i++) {
            if (bits_src.bits[i] != bits_dec.bits[i]) errors++;
        }

        printf("| %8.1f | %10d | %10d | %12.6f |\n",
            current_snr, bits_src.length, errors, (double)errors / bits_src.length);

        save_colored_output(original_text, final_text, current_snr);

        if (s == 0) {
            printf("[Info] Original: %ld bits | Huffman: %d bits | Compression: %.2f%%\n",
                fsize * 8, bits_src.length, (1.0 - (double)bits_src.length / (fsize * 8)) * 100);
        }

        free(bits_src.bits); free(bits_enc.bits);
        free(sig_tx.symbols); free(sig_rx.symbols);
        free(bits_line.bits); free(bits_line_dec.bits);
        free(bits_det.bits); free(bits_dec.bits);
        free(final_text);
    }

    free_huffman_tree(huff_root);
    free(original_text);

    printf("\nDone. Press Enter to exit...");
    (void)getchar(); 
    return 0;
}