#include <stdlib.h>

#include "harness.h"

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

int __attribute__ ((noinline)) test_harness(void) {
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

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="zedshaw-liblcthw",
        .description="some library of the book \"Learn C The Hard Way\"",
        .test_harness=*test_harness,
        .expected_runtime=380L
    };
    return _execute_harness(argc, argv, harness);
}
