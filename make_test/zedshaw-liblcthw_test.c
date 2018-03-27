#include <stdlib.h>

#include "chayai.h"

#include "zedshaw-liblcthw/src/lcthw/bstree.h"

int cmp_node(void *a, void *b) {

    return (*(int*)b) - (*(int*)a);
}

static int free_node(BSTreeNode * node)
{
    free(node->key);
    free(node->data);
    return 0;
}

BENCHMARK(zedshaw, liblcthw, 10, 1) {
    BSTree *map = BSTree_create(cmp_node);

    for(int i = 0; i < 10000; i++) {
        int *key = malloc(sizeof(int));
        int *value = malloc(sizeof(int));

        *key = i;
        *value = i;

        BSTree_set(map, key, value);
    }

    BSTree_traverse(map, free_node);

    BSTree_destroy(map);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(zedshaw, liblcthw); // some library of the book "Learn C The Hard Way"

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
