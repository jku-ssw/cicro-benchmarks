#include "harness.h"

#include "xamarin-flex/flex.h"

static inline struct flex_item* flex_item_with_size(float width, float height) {
    struct flex_item *item = flex_item_new();
    flex_item_set_width(item, width);
    flex_item_set_height(item, height);
    return item;
}

int __attribute__ ((noinline)) test_harness(void) {
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

    for(int x=0; x < 1920; x++) {
        flex_item_set_width(root, x);
        for(int y=0; y < 1080; y++) {
            flex_item_set_height(root, y);
            flex_layout(root);
        }
    }

    flex_item_free(root);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="xamarin-flex",
        .description="flexible box layout system",
        .test_harness=*test_harness,
        .expected_runtime=360L
    };
    return _execute_harness(argc, argv, harness);
}