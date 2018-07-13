#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#define NDEBUG

#include "zedshaw-liblcthw/src/lcthw/bstree.h"
#include "zedshaw-liblcthw/src/lcthw/hashmap.h"

int cmp_node(void *a, void *b) {
    return (*(int*)b) - (*(int*)a);
}

uint32_t hash(void *key) {
    return *(int*)key;
}

static int free_bstree_node(BSTreeNode * node) {
    free(node->key);
    free(node->data);
    return 0;
}

static int free_hashmap_node(HashmapNode * node) {
    free(node->key);
    free(node->data);
    return 0;
}

BENCHMARK(zedshaw_liblcthw, bstree, 100, 1) {
    BSTree *map = BSTree_create(cmp_node);
    assert(map != NULL);

    for(int i = 0; i < 5000; i++) {
        int *key = malloc(sizeof(int));
        int *value = malloc(sizeof(int));

        *key = i;
        *value = i;

        int ret = BSTree_set(map, key, value);
        assert(ret == 0);
    }

    BSTree_traverse(map, free_bstree_node);

    BSTree_destroy(map);
}

BENCHMARK(zedshaw_liblcthw, hashmap, 100, 1) {
    Hashmap *map = Hashmap_create(cmp_node, hash);
    assert(map != NULL);

    for(int i = 0; i < 50000; i++) {
        int *key = malloc(sizeof(int));
        int *value = malloc(sizeof(int));

        *key = i;
        *value = i;

        int ret = Hashmap_set(map, key, value);
        assert(ret == 0);
    }

    for(int i = 0; i < 50000; i+=10) {
        int *value = Hashmap_get(map, &i);
        assert((*value) % 5 == 0);
    }

    Hashmap_traverse(map, free_hashmap_node);

    Hashmap_destroy(map);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(zedshaw_liblcthw, bstree); // some library of the book "Learn C The Hard Way"
    REGISTER_BENCHMARK(zedshaw_liblcthw, hashmap);

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
