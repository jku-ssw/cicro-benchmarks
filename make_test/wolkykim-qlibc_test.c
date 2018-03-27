#include "chayai.h"

#include "wolkykim-qlibc/include/qlibc/qlibc.h"

BENCHMARK(wolkykim, qlibc, 10, 100) {
    qhashtbl_t *tbl = qhashtbl(0, 0);

    int i;
    for (i = 0; i < 100; i++) {
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
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(wolkykim, qlibc); // general purpose C/C++ library

    RUN_BENCHMARKS(argc, argv);

    return 0;
}