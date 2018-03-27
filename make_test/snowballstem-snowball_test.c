#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "chayai.h"

#include "snowballstem-snowball/include/libstemmer.h"

BENCHMARK(snowballstem, snowball, 10, 100) {
    struct sb_stemmer * stemmer;
    const char * language = "english";
    const char * charenc = NULL;

    stemmer = sb_stemmer_new(language, charenc);

    const char* unstemmed[10] = {"recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised"};
    for(int j=0; j < 300; j++) {
        for (int i = 0; i < 10; i++) {
            const sb_symbol *stemmed = sb_stemmer_stem(stemmer, (const sb_symbol*) unstemmed[i], sizeof("recognised"));
        }
    }

    sb_stemmer_delete(stemmer);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(snowballstem, snowball); // Snowball compiler and stemming algorithms

    RUN_BENCHMARKS(argc, argv);

    return 0;
}