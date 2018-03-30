#include <stdio.h>

#include "chayai.h"

#include "vmg-crustache/src/crustache.h"

#define TEMPLATE "<h1>{{header}}</h1>\n"\
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
    if(strncmp("header", key, key_size) == 0
       || strncmp("name", key, key_size) == 0
       || strncmp("url", key, key_size) == 0) {
        var->type = CRUSTACHE_VAR_STR;
        var->data = key;
        var->size = key_size;
    } else {
        var->type = CRUSTACHE_VAR_CONTEXT;
        var->data = "";
    }

    return 0;
}


BENCHMARK(vmg, crustache, 100, 10) {
    crustache_template* templt;
    crustache_api api = {
            crustache_context_find,
            NULL,
            NULL,
            NULL,
            NULL,
            0
    };

    int error = crustache_new(&templt, &api, TEMPLATE, sizeof(TEMPLATE));
    if(error < 0) {
        abort();
    }

    struct buf *buf = bufnew(128);

    crustache_var var;
    var.type = CRUSTACHE_VAR_CONTEXT;
    var.size = 0;


    for(int i = 0; i < 5000; i++) {
        error = crustache_render(buf, templt, &var);
        if(error < 0) {
            abort();
        }
    }

    bufrelease(buf);

    crustache_free(templt);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(vmg, crustache);

    RUN_BENCHMARKS(argc, argv);

    return 0;
}