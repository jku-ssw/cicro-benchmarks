#include "harness.h"

#include "dhess-c-ringbuf/ringbuf.h"

#define RING_BUF_LEN 419
#define TMP_BUF_LEN 40

int __attribute__ ((noinline)) test_harness(void) {
    ringbuf_t buf = ringbuf_new(RING_BUF_LEN);

    char tmp_buf[TMP_BUF_LEN] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for(int i = 0; i < 1000000; i++) {
        ringbuf_memcpy_into(buf, tmp_buf, sizeof(char) * TMP_BUF_LEN);
        ringbuf_memcpy_into(buf, tmp_buf, sizeof(char) * TMP_BUF_LEN);

        ringbuf_memcpy_from(tmp_buf, buf, sizeof(char) * (TMP_BUF_LEN / 2));
        ringbuf_memcpy_from(tmp_buf, buf, sizeof(char) * (TMP_BUF_LEN / 2));
        ringbuf_memcpy_from(tmp_buf, buf, sizeof(char) * (TMP_BUF_LEN / 2));
        ringbuf_memcpy_from(tmp_buf, buf, sizeof(char) * (TMP_BUF_LEN / 2));
    }

    ringbuf_free(&buf);
    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="dhess-c-ringbuf",
        .description="ringbuffer implementation",
        .test_harness=*test_harness,
        .expected_runtime=150L
    };
    return _execute_harness(argc, argv, harness);
}