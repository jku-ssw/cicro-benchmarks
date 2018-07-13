#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "wolkykim-qlibc/include/qlibc/qlibc.h"

BENCHMARK(wolkykim_qlibc, hashtbl, 100, 10) {
    qhashtbl_t *tbl = qhashtbl(0, 0);
    assert(tbl != NULL);

    int i;
    for (i = 0; i < 10000; i++) {
        char *key = qstrdupf("key%d%s", i, "foo");
        char *value = qstrdupf("value%d%s", i, "1a087a6982371bbfc9d4e14ae76e05ddd784a5d9c6b0fc9e6cd715baab66b90987b2ee054764e58fc04e449dfa060a68398601b64cf470cb6f0a260ec6539866");

        tbl->putstr(tbl, key, value);

        free(key);
        free(value);
    }

   for (i--; i >= 0; i--) {
        char *key = qstrdupf("key%d%s", i, "foo");
        bool ret = tbl->remove(tbl, key);
        assert(ret == true);
        free(key);
    }

    tbl->free(tbl);
}

BENCHMARK(wolkykim_qlibc, grow, 100, 10) {
    qgrow_t *grow = qgrow(0);
    assert(grow != NULL);

    // add elements
    for(int i = 0; i < 10000; i++) {
        grow->addstr(grow, "AB");       // no need to supply size
        grow->addstrf(grow, "%d", 12);  // for formatted string
        grow->addstr(grow,"CD");
    }

    char *final = grow->tostring(grow);
    assert(final != NULL);

    assert(final[6] == 'A' && final[7] == 'B' && final[8] == '1' && final[9] == '2');

    free(final);
    grow->free(grow);
}

BENCHMARK(wolkykim_qlibc, listtbl, 100, 10) {
    qlisttbl_t *tbl = qlisttbl(0);
    assert(tbl != NULL);
    char buffer[8];

    for(int i = 1; i < 1000; i++) {
        snprintf(buffer, sizeof(buffer), "e%d", i*74771 % 1151483);
        tbl->putstr(tbl, buffer, "object1");
    }

    assert(strcmp(tbl->first->name, "e74771") == 0);

    tbl->sort(tbl);

    assert(strcmp(tbl->first->name, "e100131") == 0);

    tbl->free(tbl);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(wolkykim_qlibc, hashtbl); // general purpose C/C++ library
    REGISTER_BENCHMARK(wolkykim_qlibc, grow);
    REGISTER_BENCHMARK(wolkykim_qlibc, listtbl);

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
