#include <stdlib.h>

#include "harness.h"

#include "antirez-otree/btree.h"

int __attribute__ ((noinline)) test_harness(void) {
    struct btree *bt;
    int arg = 1410065408; // TODO: value
    int count = 200000;
    uint64_t ptr;

    bt = btree_open(NULL, "./btree.db", BTREE_CREAT);
    btree_clear_flags(bt,BTREE_FLAG_USE_WRITE_BARRIER);
    if (bt == NULL) {
        perror("btree_open");
        exit(1);
    }

    for (int j = 0; j < count; j++) {
        ptr = btree_alloc(bt,arg);
        btree_free(bt,ptr);
    }

    btree_close(bt);
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="antirez-otree",
        .description="a simple btree implementation",
        .test_harness=*test_harness,
        .expected_runtime=650L
    };
    return _execute_harness(argc, argv, harness);
}
