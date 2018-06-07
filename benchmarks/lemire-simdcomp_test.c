#include <stdlib.h>

#include "chayai.h"

#include "simdcomp.h"

BENCHMARK(lemire, simdcomp, 10, 1) {
    int bit;
    size_t i;
    size_t length;
    __m128i * bb;
    srand(0);
    for (bit = 0; bit < 32; ++bit) {
        const size_t N = 2048;
        uint32_t * data = malloc(N * sizeof(uint32_t));
        uint32_t * backdata = malloc(N * sizeof(uint32_t));
        uint32_t * buffer = malloc((2 * N + 1024) * sizeof(uint32_t));

        for (i = 0; i < N; ++i) {
            data[i] = rand() & ((1 << bit) - 1);
        }
        for (length = 0; length <= N; ++length) {
            for (i = 0; i < N; ++i) {
                backdata[i] = 0;
            }
            bb = simdpack_shortlength(data, length, (__m128i *) buffer,
                                      bit);
            if((bb - (__m128i *) buffer) * sizeof(__m128i) != (unsigned) simdpack_compressedbytes(length,bit)) {
                return;
            }
            simdunpack_shortlength((__m128i *) buffer, length,
                                   backdata, bit);
            for (i = 0; i < length; ++i) {

                if (data[i] != backdata[i]) {
                    return;
                }
            }
        }
        free(data);
        free(backdata);
        free(buffer);
    }
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(lemire, simdcomp); // compressing lists of integers

    RUN_BENCHMARKS(argc, argv);

    return 0;
}