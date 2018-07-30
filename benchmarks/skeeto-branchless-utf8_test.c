#include "chayai.h"  // first include!

#include <stdlib.h>
#include <assert.h>

#include "skeeto-branchless-utf8/utf8.h"

#define IS_SURROGATE(c) ((c) >= 0xD800U && (c) <= 0xDFFFU)

#define BUFLEN 8 // MB

static uint32_t
pcg32(uint64_t *s)
{
    uint64_t m = 0x9b60933458e17d7d;
    uint64_t a = 0xd737232eeccdf7ed;
    *s = *s * m + a;
    int shift = 29 - (*s >> 61);
    return *s >> shift;
}

/* Generate a random codepoint whose UTF-8 length is uniformly selected. */
static long
randchar(uint64_t *s)
{
    uint32_t r = pcg32(s);
    int len = 1 + (r & 0x3);
    r >>= 2;
    switch (len) {
        case 1:
            return r % 128;
        case 2:
            return 128 + r % (2048 - 128);
        case 3:
            return 2048 + r % (65536 - 2048);
        case 4:
            return 65536 + r % (131072 - 65536);
    }
    abort();
}

static void *
utf8_encode(void *buf, long c)
{
    unsigned char *s = buf;
    if (c >= (1L << 16)) {
        s[0] = 0xf0 |  (c >> 18);
        s[1] = 0x80 | ((c >> 12) & 0x3f);
        s[2] = 0x80 | ((c >>  6) & 0x3f);
        s[3] = 0x80 | ((c >>  0) & 0x3f);
        return s + 4;
    } else if (c >= (1L << 11)) {
        s[0] = 0xe0 |  (c >> 12);
        s[1] = 0x80 | ((c >>  6) & 0x3f);
        s[2] = 0x80 | ((c >>  0) & 0x3f);
        return s + 3;
    } else if (c >= (1L << 7)) {
        s[0] = 0xc0 |  (c >>  6);
        s[1] = 0x80 | ((c >>  0) & 0x3f);
        return s + 2;
    } else {
        s[0] = c;
        return s + 1;
    }
}

/* Fill buffer with random characters, with evenly-distributed encoded<
 * lengths.
 */
static void *
buffer_fill(void *buf, size_t z)
{
    uint64_t s = 0;
    char *p = buf;
    char *end = p + z;
    while (p < end) {
        long c;
        do
            c = randchar(&s);
        while (IS_SURROGATE(c));
        p = utf8_encode(p, c);
    }
    return p;
}

BENCHMARK(skeeto, branchless_utf8, 100, 1) {
    size_t z = BUFLEN * 1024L * 1024;
    unsigned char *buffer = malloc(z);
    unsigned char *end = buffer_fill(buffer, z - 4);

    for(int i= 0; i < 1000; i++) {
        int e = 0;
        uint32_t c;
        utf8_decode(buffer, &c, &e);
        assert(e == 0);
    }

    assert(buffer[0] == 0xC5 && buffer[1024] == 0xA6);

    free(buffer);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(skeeto, branchless_utf8); // Branchless UTF-8 decoder

    RUN_BENCHMARKS(argc, argv);

    return 0;
}