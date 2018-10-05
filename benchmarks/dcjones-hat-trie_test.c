#include "chayai.h"  // first include!

#include <assert.h>

#include "dcjones-hat-trie/src/hat-trie.h"

#define NUM_STRINGS 10000  // how many strings
#define M_LOW 50           // minimum length of each string
#define M_HIGH 1000        // maximum length of each string

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

    char x[M_HIGH+1];

    size_t i, m;
    for (i = 0; i < NUM_STRINGS; ++i) {
        m = M_LOW + rand() % (M_HIGH - M_LOW);
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
