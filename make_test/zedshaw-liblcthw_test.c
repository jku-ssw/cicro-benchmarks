#include <stdlib.h>
#include <stdarg.h>

#include "chayai.h"

#define NDEBUG

#include "zedshaw-liblcthw/src/lcthw/bstree.h"
#include "zedshaw-liblcthw/src/lcthw/hashmap.h"

// override fprintf function, because liblcthw has some debug output on it
#define DEBUG_STR "DEBUG %s:%d:"
int fprintf(FILE * stream, const char * format, ...) {
    if(stream == stderr && strncmp(DEBUG_STR, format, strlen(DEBUG_STR)) == 0) {
        return 0;
    } else {
        va_list args;
        va_start (args, format);
        int ret = vfprintf(stream, format, args);
        va_end (args);
        return ret;
    }
}

int cmp_node(void *a, void *b) {

    return (*(int*)b) - (*(int*)a);
}

uint32_t hash(void *key) {
    return *(int*)key;
}

static int free_bstree_node(BSTreeNode * node)
{
    free(node->key);
    free(node->data);
    return 0;
}

static int free_hashmap_node(HashmapNode * node)
{
    free(node->key);
    free(node->data);
    return 0;
}

BENCHMARK(zedshaw_liblcthw, bstree, 100, 1) {
    BSTree *map = BSTree_create(cmp_node);

    for(int i = 0; i < 5000; i++) {
        int *key = malloc(sizeof(int));
        int *value = malloc(sizeof(int));

        *key = i;
        *value = i;

        BSTree_set(map, key, value);
    }

    BSTree_traverse(map, free_bstree_node);

    BSTree_destroy(map);
}

BENCHMARK(zedshaw_liblcthw, hashmap, 100, 1) {
    Hashmap *map = Hashmap_create(cmp_node, hash);

    for(int i = 0; i < 50000; i++) {
        int *key = malloc(sizeof(int));
        int *value = malloc(sizeof(int));

        *key = i;
        *value = i;

        Hashmap_set(map, key, value);
    }

    for(int i = 0; i < 50000; i+=10) {
        int *value = Hashmap_get(map, &i);
        if ((*value) % 5 != 0) {
            abort(); // should never happen,
        }
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
