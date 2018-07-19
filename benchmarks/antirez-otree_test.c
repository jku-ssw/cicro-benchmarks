#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "chayai.h"

#include "antirez-otree/btree.h"

char *filename = "btree";

BENCHMARK(otree, alloc_free, 10, 1) {
    int count = 40000;
    int retval;

    struct btree *bt = btree_open(NULL, filename, BTREE_CREAT);
    btree_clear_flags(bt, BTREE_FLAG_USE_WRITE_BARRIER);
    assert(bt != NULL);

    for (int j = 0; j < count; j++) {
        char key[BTREE_HASHED_KEY_LEN] = {0};
        snprintf(key, BTREE_HASHED_KEY_LEN, "%d", j);
        retval = btree_add(bt, (unsigned char *)key, (unsigned char *)key, strlen(key), 1);
        assert(retval == 0);
    }

    for (int j = 1000; j < 10000; j++) {
        uint64_t voff;
        uint32_t datalen;

        char key[BTREE_HASHED_KEY_LEN] = {0};
        snprintf(key, BTREE_HASHED_KEY_LEN, "%d", j);
        retval = btree_find(bt, (unsigned char *) key, &voff);
        assert(retval == 0);

        btree_alloc_size(bt, &datalen, voff);
        assert(datalen == 4);

        char *data = malloc(datalen+1);
        btree_pread(bt,(unsigned char*)data, datalen, voff);
        data[datalen] = '\0';
        assert(strcmp(data, key) == 0);
        free(data);
    }

    btree_close(bt);
}

void benchmark_cleanup (void) {
    unlink(filename); // remove file
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(otree, alloc_free); // a simple btree implementation

    // we don't want to clutter our directories
    chdir(chayai_util_get_tmp_dir());
    atexit(benchmark_cleanup);

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
