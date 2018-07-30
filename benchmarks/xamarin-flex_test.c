#include "chayai.h"  // first include!

#include <assert.h>

#include "xamarin-flex/flex.h"

static inline struct flex_item* flex_item_with_size(float width, float height) {
    struct flex_item *item = flex_item_new();
    flex_item_set_width(item, width);
    flex_item_set_height(item, height);
    return item;
}

BENCHMARK(xamarin, flex, 10, 1) {
    // from test_grow8
    struct flex_item *root = flex_item_with_size(100, 100);

    struct flex_item *child1 = flex_item_with_size(100, 10);
    flex_item_add(root, child1);

    struct flex_item *child2 = flex_item_with_size(100, 20);
    flex_item_set_grow(child2, 1);
    flex_item_add(root, child2);

    struct flex_item *child3 = flex_item_with_size(100, 20);
    flex_item_set_grow(child3, 1.5);
    flex_item_add(root, child3);

    double y_sum = 0;

    for(int x=0; x < 1920; x++) {
        flex_item_set_width(root, x);
        for(int y=0; y < 1080; y++) {
            flex_item_set_height(root, y);
            flex_layout(root);
            y_sum += flex_item_get_frame_y(child3);
        }
    }

    assert((long)y_sum == 459663360);

    flex_item_free(root);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(xamarin, flex); // flexible box layout system

    RUN_BENCHMARKS(argc, argv);

    return 0;
}