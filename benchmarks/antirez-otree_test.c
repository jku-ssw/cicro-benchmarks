#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "chayai.h"

#include "antirez-otree/btree.h"

char *filename = "btree";

BENCHMARK(otree, alloc_free, 10, 1) {
    struct btree *bt;
    int count = 10000;
    int retval;

    bt = btree_open(NULL, filename, BTREE_CREAT);
    btree_clear_flags(bt, BTREE_FLAG_USE_WRITE_BARRIER);
    assert(bt != NULL);

    for (int j = 0; j < count; j++) {
        char key[16];
        snprintf(key, 16, "%d", j);
        retval = btree_add(bt, (unsigned char *)key, (unsigned char *)key, strlen(key), 1);
        assert(retval == 0);
    }

    uint64_t voff;
    uint32_t datalen;

    char key[16];
    snprintf(key, 16, "%d", 1234);
    retval = btree_find(bt, (unsigned char*)key, &voff);
    assert(retval == 0);

    btree_alloc_size(bt, &datalen, voff);
    assert(datalen == 4);
    char *data = malloc(datalen+1);
    btree_pread(bt,(unsigned char*)data, datalen, voff);
    data[datalen] = '\0';
    assert(strcmp(data, "1234") == 0);

    btree_close(bt);
}

void benchmark_cleanup (void) {
    unlink(filename); // remove file
}

int main(int argc, char** argv) {
    REGISTER_BENCHMARK(otree, alloc_free); // a simple btree implementation

    atexit(benchmark_cleanup);

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
