
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "sort.h"

_attribute_ ((noinline))

int benchmark(int problemSize);
int getProblemsize();

int main() {
    int result = 0;
    struct timeval tv_start = (struct timeval ) malloc(sizeof(struct timeval));
    struct timeval tv_end = (struct timeval ) malloc(sizeof(struct timeval));

    int start = 0;
    int end = 0;
    long time;
    int i;
    int totalCount = 50;
    int problemSize;
    for (i = 0; i < totalCount; i++) {
        problemSize = getProblemsize();
        gettimeofday(tv_start, NULL);
        result = benchmark(problemSize);
        gettimeofday(tv_end, NULL);
        time = (tv_end->tv_sec-tv_start->tv_sec)*1000L+(long)((tv_end->tv_usec-tv_start->tv_usec)*0.001);
        printf("execution time: %ld\n", time);
    }
    free(tv_start);
    free(tv_end);
    fflush(stdout);
    return 0;
}

static void fill(int64_t *arr, const int size) {
  int i;

  for (i = 0; i < size; i++) {
    arr[i] = lrand48();
  }
}

/* Used to control the demo */
#define SEED 123
#define SIZE 400
#define RUNS 20

int benchmark(int problemSize) {
    int64_t arr[SIZE];
    int64_t dst[SIZE];

    srand48(SEED);
    for (i = 0; i < RUNS; i++) {
        fill(arr, problemSize);
        memcpy(dst, arr, sizeof(int64_t) * problemSize);
        //qsort(dst, problemSize, sizeof(int64_t), simple_cmp);
        sorter_quick_sort(dst, problemSize);
    }
}

int getProblemsize() {
    return SIZE;
}
