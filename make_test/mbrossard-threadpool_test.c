#include "harness.h"

#include <pthread.h>
#include "mbrossard-threadpool/src/threadpool.h"


#define THREAD 4
#define SIZE   8192*8192

threadpool_t *pool;

int left;
pthread_mutex_t lock;

void dummy_task(void *arg) {
    pthread_mutex_lock(&lock);
    left--;
    pthread_mutex_unlock(&lock);
}

int __attribute__ ((noinline)) test_harness(void) {
    int i;

    pthread_mutex_init(&lock, NULL);

    left = SIZE;
    pool = threadpool_create(THREAD, SIZE, 0);
    for(i = 0; i < SIZE; i++) {
        threadpool_add(pool, &dummy_task, NULL, 0);
    }
    threadpool_destroy(pool, threadpool_graceful);

    pthread_mutex_destroy(&lock);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="mbrossard-threadpool",
        .description="Simple Thread pool implementation",
        .test_harness=*test_harness,
        .expected_runtime=220L
    };
    return _execute_harness(argc, argv, harness);
}