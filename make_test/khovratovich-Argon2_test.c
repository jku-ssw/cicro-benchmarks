#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "harness.h"

#include "khovratovich-Argon2/Source/C99/Argon2/argon2.h"

int __attribute__ ((noinline)) test_harness(void) {

    const uint32_t inlen = 16;
    const unsigned outlen=16;
    unsigned char out[outlen];
    unsigned char pwd_array[inlen];
    unsigned char salt_array[inlen];

    uint32_t t_cost = 1;

    memset(pwd_array, 0, inlen);
    memset(salt_array, 1, inlen);

    uint32_t m_cost = 1 << 14;
    uint32_t thread_n = 1;

    Argon2_Context context = {out, outlen, pwd_array, inlen, salt_array, inlen,
                              NULL, 0, NULL, 0, t_cost, m_cost, thread_n, thread_n, NULL, NULL, false, false, false,false };
    Argon2d(&context);

    Argon2i(&context);

    Argon2id(&context);

    Argon2ds(&context);

    return 0;
}

int main(int argc, char* argv[]) {
    _test_harness harness = {
        .name="khovratovich-Argon2",
        .description="Memory-hard scheme Argon2",
        .test_harness=*test_harness,
        .expected_runtime=470L
    };
    return _execute_harness(argc, argv, harness);
}
