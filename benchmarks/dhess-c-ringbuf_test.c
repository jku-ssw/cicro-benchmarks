#include <assert.h>

#include "chayai.h"

#include "dhess-c-ringbuf/ringbuf.h"

#define RING_BUF_LEN 419

BENCHMARK(dhess, c_ringbuf, 100, 100) {
    ringbuf_t buf = ringbuf_new(RING_BUF_LEN);
    assert(buf != 0);

    char tmp_buf[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                      0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for(int i = 0; i < 1000; i++) {
        ringbuf_memcpy_into(buf, tmp_buf, sizeof(tmp_buf));
        ringbuf_memcpy_into(buf, tmp_buf, sizeof(tmp_buf));

        ringbuf_memcpy_from(tmp_buf, buf, sizeof(tmp_buf) / 2);
        ringbuf_memcpy_from(tmp_buf+2, buf, sizeof(tmp_buf) / 2);
        ringbuf_memcpy_from(tmp_buf+4, buf, sizeof(tmp_buf) / 2);
        ringbuf_memcpy_from(tmp_buf+8, buf, sizeof(tmp_buf) / 2);
    }

    assert(tmp_buf[0] == 0 && tmp_buf[2] == 2 &&  tmp_buf[4] == 0 && tmp_buf[6] == 2);

    ringbuf_free(&buf);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(dhess, c_ringbuf); // ringbuffer implementation

    RUN_BENCHMARKS(argc, argv);

    return 0;
}