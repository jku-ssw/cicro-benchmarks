#include "harness.h"

#include "wolkykim-qlibc/include/qlibc/qlibc.h"

int __attribute__ ((noinline)) test_harness(void) {
    qhashtbl_t *tbl = qhashtbl(0, 0);

    int i;
    for (i = 0; i < 100000; i++) {
        char *key = qstrdupf("key%d%s", i, "foo");
        char *value = qstrdupf("value%d%s", i, "1a087a6982371bbfc9d4e14ae76e05ddd784a5d9c6b0fc9e6cd715baab66b90987b2ee054764e58fc04e449dfa060a68398601b64cf470cb6f0a260ec6539866");

        tbl->putstr(tbl, key, value);

        free(key);
        free(value);
    }

   for (i--; i >= 0; i--) {
        char *key = qstrdupf("key%d%s", i, "foo");
        tbl->remove(tbl, key);
        free(key);
    }

    tbl->free(tbl);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="wolkykim-qlibc",
        .description="general purpose C/C++ library",
        .test_harness=*test_harness,
        .expected_runtime=410L
    };
    return _execute_harness(argc, argv, harness);
}