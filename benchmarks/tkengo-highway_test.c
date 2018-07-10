#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "chayai.h"

#include "oniguruma.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

BENCHMARK(tkengo, highway, 10, 5) {
    int r;
    unsigned char *start, *range, *end;
    regex_t* reg;
    OnigErrorInfo einfo;
    OnigRegion *region;

    static UChar* pattern = (UChar* )"(.{20}d|.{12})a.+m.*e.{10}o";
    static UChar* str = (UChar* )LOREM_IPSUM1000;

    r = onig_new(&reg, pattern, pattern + strlen((char* )pattern),
                 ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, &einfo);
    assert(r == ONIG_NORMAL);

    region = onig_region_new();

    end   = str + strlen((char* )str);
    start = str;
    range = end;

    int sum_r = 0;
    do {
        r = onig_search(reg, str, end, start, range, region, ONIG_OPTION_NONE);
        start += r + 1;
        sum_r += r;
    } while(r >= 1);

    assert(r == -1 && sum_r == 1706461);

    onig_region_free(region, 1 /* 1:free self, 0:free contents only */);
    onig_free(reg);
    onig_end();
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(tkengo, highway); // High performance source code search tool

    RUN_BENCHMARKS(argc, argv);

    return 0;
}