#include "chayai.h"  // first include!

#include <assert.h>

#include "dcjones-hat-trie/src/hat-trie.h"

/* Simple random string generation. */
void randstr(char* x, size_t len)
{
    x[len] = '\0';
    while (len > 0) {
        x[--len] = '\x20' + (rand() % ('\x7e' - '\x20' + 1));
    }
}

BENCHMARK(dcjones, hat_trie, 100, 1) {
    hattrie_t* T = hattrie_create();
    assert(T != NULL);

    const size_t n = 10000;  // how many strings
    const size_t m_low  = 50;  // minimum length of each string
    const size_t m_high = 1000; // maximum length of each string
    char x[m_high+1];

    size_t i, m;
    for (i = 0; i < n; ++i) {
        m = m_low + rand() % (m_high - m_low);
        randstr(x, m);
        *hattrie_get(T, x, m) = 1;
    }

    assert(hattrie_size(T) == 10000);

    hattrie_iter_t* it;
    clock_t t0, t;
    const size_t repetitions = 16;
    size_t r;

    for (r = 0; r < repetitions; ++r) {
        it = hattrie_iter_begin(T, false);
        while (!hattrie_iter_finished(it)) {
            hattrie_iter_next(it);
        }
        hattrie_iter_free(it);
    }

    hattrie_free(T);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(dcjones, hat_trie); // An efficient trie implementation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}