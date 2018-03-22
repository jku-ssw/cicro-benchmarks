#include <stdlib.h>

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
    /* Insert element */
    for (int i = 0; i < 10000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        raxInsert(t,(unsigned char*)buf,len,(void*)(long)i,NULL);
    }

    /* Linear lookup */
    for (int i = 0; i < 10000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        void *data = raxFind(t,(unsigned char*)buf,len);
    }

    /* Random lookup */
    for (int i = 0; i < 10000; i++) {
        char buf[64];
        int r = rand() % 10000;
        int len = int2key(buf,sizeof(buf),r);
        void *data = raxFind(t,(unsigned char*)buf,len);
    }

    /* Deletion */
    for (int i = 0; i < 10000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        int retval = raxRemove(t,(unsigned char*)buf,len,NULL);
    }

    raxFree(t);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(rax, tree); // radix tree implementation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}