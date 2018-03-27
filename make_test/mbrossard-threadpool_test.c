#include "chayai.h"

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

BENCHMARK(mbrossard, threadpool, 10, 1) {
    int i;

    pthread_mutex_init(&lock, NULL);

    left = SIZE;
    pool = threadpool_create(THREAD, SIZE, 0);
    for(i = 0; i < SIZE; i++) {
        threadpool_add(pool, &dummy_task, NULL, 0);
    }
    threadpool_destroy(pool, threadpool_graceful);

    pthread_mutex_destroy(&lock);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(mbrossard, threadpool); // Simple Thread pool implementation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}