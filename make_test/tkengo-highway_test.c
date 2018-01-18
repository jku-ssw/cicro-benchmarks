#include <stdio.h>
#include <string.h>
#include "oniguruma.h"

#include "harness.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

int __attribute__ ((noinline)) test_harness(void) {

    int r;
    unsigned char *start, *range, *end;
    regex_t* reg;
    OnigErrorInfo einfo;
    OnigRegion *region;

    static UChar* pattern = (UChar* )"(.{20}d|.{12}a)a.+m.*e.{10}o";
    static UChar* str     = (UChar* )LOREM_IPSUM1000;

    r = onig_new(&reg, pattern, pattern + strlen((char* )pattern),
                 ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, &einfo);

    region = onig_region_new();

    end   = str + strlen((char* )str);
    start = str;
    range = end;
    do {
        r = onig_search(reg, str, end, start, range, region, ONIG_OPTION_NONE);

        if (r >= 0) {
            int i;

            //fprintf(stderr, "match at %d\n", r);
        }
        start = start + r + 1;
    } while(r >= 1);



    onig_region_free(region, 1 /* 1:free self, 0:free contents only */);
    onig_free(reg);
    onig_end();

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="tkengo-highway",
        .description=" High performance source code search tool",
        .test_harness=*test_harness,
        .expected_runtime=500L
    };
    return _execute_harness(argc, argv, harness);
}
