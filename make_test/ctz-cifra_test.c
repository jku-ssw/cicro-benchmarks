#include <string.h>
#include <stdlib.h>

#include "chayai.h"

#include "ctz-cifra/src/sha3.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

BENCHMARK(cifra, sha3_224, 100, 1) {
    uint8_t out_sha3_224[CF_SHA3_224_HASHSZ];
    cf_hash(&cf_sha3_224, LOREM_IPSUM1000, sizeof(LOREM_IPSUM1000), out_sha3_224);
}

BENCHMARK(cifra, sha3_256, 100, 1) {
    uint8_t out_sha3_256[CF_SHA3_256_HASHSZ];
    cf_hash(&cf_sha3_256, LOREM_IPSUM1000, sizeof(LOREM_IPSUM1000), out_sha3_256);
}

BENCHMARK(cifra, sha3_384, 100, 1) {
    uint8_t out_sha3_384[CF_SHA3_384_HASHSZ];
    cf_hash(&cf_sha3_384, LOREM_IPSUM1000, sizeof(LOREM_IPSUM1000), out_sha3_384);
}

BENCHMARK(cifra, sha3_512, 100, 1) {
    uint8_t out_sha3_512[CF_SHA3_512_HASHSZ];
    cf_hash(&cf_sha3_512, LOREM_IPSUM1000, sizeof(LOREM_IPSUM1000), out_sha3_512);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(cifra, sha3_224); // A collection of cryptographic primitives targeted at embedded use
    REGISTER_BENCHMARK(cifra, sha3_256);
    REGISTER_BENCHMARK(cifra, sha3_384);
    REGISTER_BENCHMARK(cifra, sha3_512);

    RUN_BENCHMARKS(argc, argv);

    return 0;
}