#include <stdlib.h>
#include <assert.h>

#include "chayai.h"

#include "antirez-rax/rax.h"

static size_t int2key(char *s, size_t maxlen, uint32_t i) {
        if (maxlen > 16) maxlen = 16;
        int r = rand() % maxlen;
        for (int i = 0; i < r; i++) s[i] = rand()&0xff;
        return r;
}

BENCHMARK(rax, tree, 100, 1) {
    rax *t = raxNew();
    assert(t != NULL);

    srand(0);
    /* Insert element */
    for (int i = 0; i < 10000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        int ret = raxInsert(t,(unsigned char*)buf,len,(void*)(long)i,NULL);
    }

    srand(0);
    /* Linear lookup */
    for (int i = 0; i < 10000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        void *data = raxFind(t,(unsigned char*)buf,len);
        assert(data != raxNotFound);
    }

    srand(0);
    /* Random lookup */
    for (int i = 0; i < 10000; i++) {
        char buf[64];
        int r = rand() % 10000;
        int len = int2key(buf,sizeof(buf),r);
        void *data = raxFind(t,(unsigned char*)buf,len);
    }

    assert(t->numele >= 1000 && t->numnodes >= 1000); // 9004 and 17545 on my system, but we cannot be sure due to rng

    srand(0);
    /* Deletion */
    for (int i = 0; i < 10000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        int retval = raxRemove(t,(unsigned char*)buf,len,NULL);
    }

    assert(t->numele == 0 && t->numnodes == 1);

    raxFree(t);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(rax, tree); // radix tree implementation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
