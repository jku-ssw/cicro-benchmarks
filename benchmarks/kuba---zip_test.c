#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "chayai.h"

#include "kuba---zip/src/zip.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

#define FILENAME "foo.zip"
char *filename = NULL;

BENCHMARK(kuba, zip, 10, 1) {
    assert(filename != NULL);
    struct zip_t *zip = zip_open(filename, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

    zip_entry_open(zip, "foo-1.txt");
    {
        char *data = LOREM_IPSUM1000;
        size_t len = strlen(LOREM_IPSUM1000);
        for(int i = 0; i < 20; i++) {
            zip_entry_write(zip, data, len);
            data += i;
            len -= i;
        }
    }
    zip_entry_close(zip);

    assert(zip_total_entries(zip) == 1);

    zip_close(zip);
}

void benchmark_cleanup (void) {
    unlink(filename); // remove file

    free(filename);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(kuba, zip); // simple zip library

    filename = malloc(1024*sizeof(char));
    sprintf(filename, "%s/%s", chayai_util_get_tmp_dir(), FILENAME);
    atexit(benchmark_cleanup);

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
