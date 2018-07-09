#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "chrismoos-hash-ring/hash_ring.h"

// hash_ring_test.c

void fillRandomBytes(uint8_t *bytes, uint32_t num) {
    int x;
    for(x = 0; x < num; x++) {
        bytes[x] = (rand() % 25) + 97;
    }
}

void addNodes(hash_ring_t *ring, int numNodes) {
    int x;
    uint8_t nodeName[16];

    for(x = 0; x < numNodes; x++) {
        fillRandomBytes(nodeName, sizeof(nodeName));
        hash_ring_add_node(ring, nodeName, sizeof(nodeName));
    }
}

void generateKeys(uint8_t *keys, int numKeys, int keySize) {
    int x;
    for(x = 0; x < numKeys; x++) {
        fillRandomBytes(&keys[x], keySize);
    }
}

#define NUM_REPLICAS 512
#define NUM_NODES 64
#define NUM_KEYS 1000
#define KEY_SIZE 16

BENCHMARK(chrismoos, hash_ring, 100, 1) {
    hash_ring_t *ring = hash_ring_create(NUM_REPLICAS, HASH_FUNCTION_SHA1);
    assert(ring != NULL);
    assert(ring->numNodes == 0 && ring->numItems == 0);

    addNodes(ring, NUM_NODES);
    assert(ring->numNodes == 64 && ring->numItems == 32768);

    uint8_t *keys = (uint8_t*)malloc(KEY_SIZE * NUM_KEYS);
    generateKeys(keys, NUM_KEYS, KEY_SIZE);

    for(int x = 0; x < NUM_KEYS; x++) {
        hash_ring_node_t *node = hash_ring_find_node(ring, keys + (KEY_SIZE * x), KEY_SIZE);
        assert(node != NULL);
    }

    free(keys);
    hash_ring_free(ring);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(chrismoos, hash_ring); // hash ring library

    RUN_BENCHMARKS(argc, argv);

    return 0;
}