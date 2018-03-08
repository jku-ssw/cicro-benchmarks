#include <stdio.h>

#include "harness.h"

#include "vmg-crustache/src/crustache.h"

#define TEMPLATE "asdf ASDF\0"

#define TEMPLATE2 "<h1>{{header}}</h1>\n"\
"{{#bug}}\n"\
"{{/bug}}\n"\
"\n"\
"{{#items}}\n"\
"  {{#first}}\n"\
"    <li><strong>{{name}}</strong></li>\n"\
"  {{/first}}\n"\
"  {{#link}}\n"\
"    <li><a href=\"{{url}}\">{{name}}</a></li>\n"\
"  {{/link}}\n"\
"{{/items}}\n"\
"\n"\
"{{#empty}}\n"\
"  <p>The list is empty.</p>\n"\
"{{/empty}}"

int crustache_context_find(crustache_var *var, void *context, const char *key, size_t key_size) {
    fprintf(stderr, "crustache_context_find\n");
    return 0;
}

int crustache_list_get(crustache_var *var, void *list, size_t i) {
    fprintf(stderr, "crustache_list_get\n");
    return 0;
}

int crustache_lambda(crustache_var *var, void *lambda, const char *raw_template, size_t raw_size) {
    fprintf(stderr, "crustache_lambda\n");
    return 0;
}

void crustache_var_free(crustache_var_t type, void *var) {
    fprintf(stderr, "crustache_var_free\n");
}

int crustache_partial(crustache_template **partial, const char *partial_name, size_t name_size) {
    fprintf(stderr, "crustache_partial\n");
    return 0;
}


int __attribute__ ((noinline)) test_harness(void) {

    crustache_template* templt;
    crustache_api api = {
            crustache_context_find,
            crustache_list_get,
            crustache_lambda,
            crustache_var_free,
            crustache_partial,
            0
    };

    crustache_new(&templt, &api, TEMPLATE, sizeof(TEMPLATE));



    struct buf buf;
    buf.size = 0;

    crustache_var var;
    var.type = CRUSTACHE_VAR_CONTEXT;
    var.size = 0;

    int error = crustache_render(&buf, templt, &var);
    printf("return code: %d\n", error);
    printf("%d \n", buf.size);

    crustache_free(templt);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="vmg-crustache",
        .description="", // TODO: missing
        .test_harness=*test_harness,
        .expected_runtime=0L // TODO: missing (ms)
    };
    return _execute_harness(argc, argv, harness);
}