#include <unistd.h>
#include <assert.h>

#include "chayai.h"

#include <pthread.h>
#include "mbrossard-threadpool/src/threadpool.h"


#define THREAD 1
#define SIZE   8192 //*8192

int left;
pthread_mutex_t lock;

void dummy_task(void *arg) {
    pthread_mutex_lock(&lock);
    left--;
    pthread_mutex_unlock(&lock);
}

BENCHMARK(mbrossard, threadpool, 100, 100) {
    int ret, i, copy=1;

    pthread_mutex_init(&lock, NULL);

    left = SIZE;

    threadpool_t *pool = threadpool_create(THREAD, SIZE, 0);
    assert(pool != NULL);

    for(i = 0; i < SIZE; i++) {
        ret = threadpool_add(pool, &dummy_task, NULL, 0);
        assert(ret == 0);
    }

    ret = threadpool_destroy(pool, threadpool_graceful);
    assert(ret == 0);

    while(copy > 0) {
        usleep(10);
        pthread_mutex_lock(&lock);
        copy = left;
        pthread_mutex_unlock(&lock);
    }

    assert(left == 0);

    pthread_mutex_destroy(&lock);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(mbrossard, threadpool); // Simple Thread pool implementation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}