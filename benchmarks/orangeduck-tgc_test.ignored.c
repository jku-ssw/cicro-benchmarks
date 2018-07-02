#include "chayai.h"

#include "orangeduck-tgc/tgc.h"

#define LIST_SIZE 10000

static tgc_t gc;

struct list {
    struct list* next;
    int val;
};

static void do_some_linked_list_stuff() {
    struct list *head = tgc_alloc(&gc, sizeof(struct list));

    struct list *tail = head;

    // init linked list
    for(int i = 0; i < LIST_SIZE; i++) {
        struct list *new_tail = tgc_alloc(&gc, sizeof(struct list));
        tail->next = new_tail;
        tail->val = i;
        tail = new_tail;
    }

    // delete some items at the start of list
    for(int i = 0; i < 10; i++) {
        head = head->next;
    }

    // delete an item in the middle
    struct list *tmp = head;
    for(int i = 0; i < 10; i++) {
        tmp = tmp->next;
    }
    tmp->next = tmp->next->next;
}

BENCHMARK(orangeduck, tgc, 10, 10) {
    int local_var;
    tgc_start(&gc, &local_var);

    tgc_pause(&gc);
    do_some_linked_list_stuff();
    //tgc_resume(&gc);
    tgc_run(&gc);

    tgc_stop(&gc);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(orangeduck, tgc); // mark and sweep gabage collector

    RUN_BENCHMARKS(argc, argv);

    return 0;
}