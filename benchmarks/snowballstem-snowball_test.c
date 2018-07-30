#include "chayai.h"  // first include!

#include <string.h>
#include <stddef.h>
#include <assert.h>

#include "snowballstem-snowball/include/libstemmer.h"

volatile const char* unstemmed[10] = {"recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised"};

BENCHMARK(snowballstem, snowball, 100, 100) {
    struct sb_stemmer * stemmer;
    const char * language = "english";
    const char * charenc = NULL;

    stemmer = sb_stemmer_new(language, charenc);

    for(int j=0; j < 100; j++) {
        for (int i = 0; i < 10; i++) {
            const sb_symbol *stemmed = sb_stemmer_stem(stemmer, (const sb_symbol*) unstemmed[i], strlen((const char *)unstemmed[i]));
            assert(strcmp((const char *)stemmed, "recognis") == 0);
        }
    }

    sb_stemmer_delete(stemmer);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(snowballstem, snowball); // Snowball compiler and stemming algorithms

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
