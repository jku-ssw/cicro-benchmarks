#include <stdlib.h>

#include "harness.h"

#include "antirez-rax/rax.h"

static size_t int2key(char *s, size_t maxlen, uint32_t i) {
        if (maxlen > 16) maxlen = 16;
        int r = rand() % maxlen;
        for (int i = 0; i < r; i++) s[i] = rand()&0xff;
        return r;
}

int __attribute__ ((noinline)) test_harness(void) {
    rax *t = raxNew();
    /* Insert element */
    for (int i = 0; i < 100000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        raxInsert(t,(unsigned char*)buf,len,(void*)(long)i,NULL);
    }

    /* Linear lookup */
    for (int i = 0; i < 100000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        void *data = raxFind(t,(unsigned char*)buf,len);
    }

    /* Random lookup */
    for (int i = 0; i < 100000; i++) {
        char buf[64];
        int r = rand() % 100000;
        int len = int2key(buf,sizeof(buf),r);
        void *data = raxFind(t,(unsigned char*)buf,len);
    }

    /* Deletion */
    for (int i = 0; i < 100000; i++) {
        char buf[64];
        int len = int2key(buf,sizeof(buf),i);
        int retval = raxRemove(t,(unsigned char*)buf,len,NULL);
    }

    raxFree(t);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="antirez-rax",
        .description="radix tree implementation",
        .test_harness=*test_harness,
        .expected_runtime=180L
    };
    return _execute_harness(argc, argv, harness);
}
