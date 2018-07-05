#include "chayai.h"

#include "index.h"
#include "store.h"
#include "search.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// override printf function, because Cranium outputs by default to it
int printf(const char * format, ...) {
    return 0;
}

void create_index(FrtStore *store, FrtConfig config)
{
    // based on c/sort_bench.c
    int i;
    FrtIndexWriter *iw;
    FrtAnalyzer *a = frt_whitespace_analyzer_new(true);
    FrtFieldInfos *fis = frt_fis_new(FRT_STORE_NO, FRT_INDEX_UNTOKENIZED, FRT_TERM_VECTOR_NO);
    char data[1000];
    FrtDocument *doc;
    frt_index_create(store, fis);
    frt_fis_deref(fis);

    iw = frt_iw_open(store, a, &config);


    //config.merge_factor = 10;
    //config.min_merge_docs = 1000;

    for (i = 0; i < 99999; i++) {
        if (i%10000 == 0) printf("up to %d\n", i);
        doc = frt_doc_new();
        sprintf(data, "<%d>", rand() % 1000000);
        frt_doc_add_field(doc, frt_df_add_data(frt_df_new((FrtSymbol)"num"), data));
        frt_iw_add_doc(iw, doc);
        frt_doc_destroy(doc);
    }
    //iw_optimize(iw);
    frt_iw_close(iw);
}

BENCHMARK(dbalmain, ferret, 10, 1) {
    FrtConfig config = frt_default_config;
    FrtStore *store = frt_open_fs_store("./tmp/dbalmain-ferret/"); // TODO: a few blocks are not free at the end?
    FrtSearcher *searcher;
    FrtSort *sort = frt_sort_new();
    FrtQuery *q = frt_tq_new((FrtSymbol)"num", "<1234>");
    FrtTopDocs *td;
    config.max_field_length = 0x7FFFFFFF;
    config.max_buffer_memory = 0x40000000;
    config.chunk_size = 0x8000000;
    config.max_buffered_docs = 1000;
    config.merge_factor = 11;

    create_index(store, config);

    // TODO: frt_ir_open currently segfaults because of unknown reasons
    /*
    searcher = frt_isea_new(frt_ir_open(store));

    frt_sort_add_sort_field(sort, frt_sort_field_int_new((FrtSymbol)"num", false));
    td = frt_searcher_search(searcher, q, 0, 10, NULL, sort, NULL);
    frt_td_destroy(td);
    frt_q_deref(q);

    q = frt_rq_new((FrtSymbol)"num", "<100000>", "<800000>", true, true);
    td = frt_searcher_search(searcher, q, 0, 10, NULL, NULL, NULL);
    frt_td_destroy(td);

    frt_searcher_close(searcher);
    */

    frt_q_deref(q);
    frt_sort_destroy(sort);
    frt_store_deref(store);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(dbalmain, ferret); // extensible information retrieval library

    RUN_BENCHMARKS(argc, argv);

    return 0;
}