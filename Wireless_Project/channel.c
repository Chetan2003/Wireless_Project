#include <stdlib.h>
#include <math.h>
#include "wireless.h"

// Internal helper (Same as before)
double generate_gaussian_noise(double snr_db) {
    // Note: For QPSK with Energy=1, noise power calculation remains similar
    // However, noise is split between I and Q. 
    // Usually, N0 is total noise power. Sigma per dimension = sqrt(N0/2).
    // To keep it simple for this class:
    // SNR = Es / N0. Es = 1. N0 = 10^(-SNR/10).
    // Sigma (for one dimension) = sqrt(N0 / 2).

    double snr_linear = pow(10.0, snr_db / 10.0);
    double n0 = 1.0 / snr_linear;
    double sigma = sqrt(n0 / 2.0); // Split noise power between I and Q

	//box muller transform to generate Gaussian noise
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    if (u1 < 1e-10) u1 = 1e-10;

    return sigma * sqrt(-2.0 * log(u1)) * cos(2.0 * 3.1415926535 * u2);
}

SignalStream awgn_channel(SignalStream input, double snr_db) {
    SignalStream output;
    output.length = input.length;
    output.symbols = (Complex*)malloc(output.length * sizeof(Complex));

    for (int i = 0; i < input.length; i++) {
        // Add independent noise to I and Q
        output.symbols[i].I = input.symbols[i].I + generate_gaussian_noise(snr_db);
        output.symbols[i].Q = input.symbols[i].Q + generate_gaussian_noise(snr_db);
    }
    return output;
}