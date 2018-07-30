#include "chayai.h"  // first include!

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "ctz-cifra/src/sha3.h"
#include "ctz-cifra/src/salsa20.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

BENCHMARK(cifra, sha3_224, 100, 1) {
    uint8_t out[CF_SHA3_224_HASHSZ];
    cf_hash(&cf_sha3_224, LOREM_IPSUM1000, sizeof(LOREM_IPSUM1000), out);
    assert(out[0] == 0x6E && out[16] == 0xDC && out[27] == 0x87);
}

BENCHMARK(cifra, sha3_256, 100, 1) {
    uint8_t out[CF_SHA3_256_HASHSZ];
    cf_hash(&cf_sha3_256, LOREM_IPSUM1000, sizeof(LOREM_IPSUM1000), out);
    assert(out[0] == 0x45 && out[16] == 0x62 && out[31] == 0xE9);
}

BENCHMARK(cifra, sha3_384, 100, 1) {
    uint8_t out[CF_SHA3_384_HASHSZ];
    cf_hash(&cf_sha3_384, LOREM_IPSUM1000, sizeof(LOREM_IPSUM1000), out);
    assert(out[0] == 0x81 && out[32] == 0xDC && out[47] == 0xD8);
}

BENCHMARK(cifra, sha3_512, 100, 1) {
    uint8_t out[CF_SHA3_512_HASHSZ];
    cf_hash(&cf_sha3_512, LOREM_IPSUM1000, sizeof(LOREM_IPSUM1000), out);
    assert(out[0] == 0x77 && out[32] == 0x8C && out[63] == 0x39);
}

BENCHMARK(cifra, chacha20, 100, 1) {
    uint8_t key[32] = {1};
    uint8_t nonce[8] = {1};
    uint8_t *block = malloc(sizeof(LOREM_IPSUM1000) * sizeof(uint8_t));

    memset(block, 0, 256);

    cf_chacha20_ctx ctx;
    cf_chacha20_init(&ctx, key, sizeof(key), nonce);
    cf_chacha20_cipher(&ctx, (const uint8_t *)LOREM_IPSUM1000, block, sizeof(LOREM_IPSUM1000));

    assert(block[0] == 0xAA && block[256] == 0xF6 && block[512] == 0x8C);

    free(block);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(cifra, sha3_224); // A collection of cryptographic primitives targeted at embedded use
    REGISTER_BENCHMARK(cifra, sha3_256);
    REGISTER_BENCHMARK(cifra, sha3_384);
    REGISTER_BENCHMARK(cifra, sha3_512);

    REGISTER_BENCHMARK(cifra, chacha20);

    RUN_BENCHMARKS(argc, argv);

    return 0;
}