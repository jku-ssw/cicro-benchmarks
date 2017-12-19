#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "harness.h"

#include "snowballstem-snowball/include/libstemmer.h"

int __attribute__ ((noinline)) test_harness(void) {
    struct sb_stemmer * stemmer;
    const char * language = "english";
    const char * charenc = NULL;

    stemmer = sb_stemmer_new(language, charenc);

    const char* unstemmed[10] = {"recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised", "recognised"};
    for(int j=0; j < 300000; j++) { // TODO: that's a way to tight loop I think
        for (int i = 0; i < 10; i++) {
            const sb_symbol *stemmed = sb_stemmer_stem(stemmer, (const sb_symbol*) unstemmed[i], sizeof("recognised"));
        }
    }

    sb_stemmer_delete(stemmer);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="snowballstem-snowball",
        .description="Snowball compiler and stemming algorithms",
        .test_harness=*test_harness,
        .expected_runtime=500L
    };
    return _execute_harness(argc, argv, harness);
}
