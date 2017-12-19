#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "harness.h"

#include "troydhanson-tpl/src/tpl.h"

#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
#define LOREM_IPSUM10 LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM LOREM_IPSUM
#define LOREM_IPSUM100 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10 LOREM_IPSUM10
#define LOREM_IPSUM1000 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100 LOREM_IPSUM100

volatile uint8_t *str;

int __attribute__ ((noinline)) test_harness(void) {
    tpl_node *tn, *tnl;
    int id;
    char *name, *names[] = { LOREM_IPSUM, "LOREM_IPSUM10", "LOREM_IPSUM1000" };

    tn = tpl_map("A(is)", &id, &name);
    tnl = tpl_map("A(is)", &id, &name);

    name = LOREM_IPSUM1000;
    for(id=0; id < 1024; id++) {
        for (int i = 0; i < 8; i++, id++) {

            name += i*3;
            tpl_pack(tn, 1);
        }
    }

    void* addr;
    size_t len;
    tpl_dump(tn, TPL_MEM, &addr, &len);

    tpl_load(tnl, TPL_MEM, addr, len);

    while ( tpl_unpack(tnl,1) > 0 ) {
        free(name);
    }

    free(addr);
    tpl_free(tnl);
    tpl_free(tn);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="troydhanson-tpl",
        .description=" small binary serialization library",
        .test_harness=*test_harness,
        .expected_runtime=410L
    };
    return _execute_harness(argc, argv, harness);
}
