#include <stdlib.h>

#include "chayai.h"

#include "antirez-otree/btree.h"

BENCHMARK(otree, alloc_free, 100, 10) {
    struct btree *bt;
    int arg = 1410065408; // TODO: value
    int count = 2000;
    uint64_t ptr;

    bt = btree_open(NULL, "./btree.db", BTREE_CREAT); // TODO: store in tmp directory or even in RAM
    btree_clear_flags(bt, BTREE_FLAG_USE_WRITE_BARRIER);
    if (bt == NULL) {
        abort();
    }

    for (int j = 0; j < count; j++) { // TODO: do some more sophisticated that alloc and free behind each other
        ptr = btree_alloc(bt,arg);
        btree_free(bt,ptr);
    }

    btree_close(bt);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(otree, alloc_free); // a simple btree implementation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}