#include "chayai.h"  // first include!

#include <string.h>
#include <assert.h>

#include "khovratovich-Argon2/Source/C99/Argon2/argon2.h"

#define CREATE_CONTEXT \
    const uint32_t inlen = 16;\
    const unsigned outlen=16;\
    unsigned char out[outlen];\
    unsigned char pwd_array[inlen];\
    unsigned char salt_array[inlen];\
    uint32_t t_cost = 1;\
    memset(pwd_array, 0, inlen);\
    memset(salt_array, 1, inlen);\
    uint32_t m_cost = 1 << 18;\
    uint32_t thread_n = 1;\
    Argon2_Context context = {out, outlen, pwd_array, inlen, salt_array, inlen,\
                              NULL, 0, NULL, 0, t_cost, m_cost, thread_n, thread_n, NULL, NULL, false, false, false,false }

BENCHMARK(khovratovich, Argon2d, 10, 1) {
    CREATE_CONTEXT;

    Argon2d(&context);

    assert(out[0] == 0x41 && out[8] == 0x33 && out[15] == 0xD0);
}

BENCHMARK(khovratovich, Argon2i, 10, 1) {
    CREATE_CONTEXT;

    Argon2i(&context);

    assert(out[0] == 0x82 && out[8] == 0xA7 && out[15] == 0x3C);
}

BENCHMARK(khovratovich, Argon2id, 10, 1) {
    CREATE_CONTEXT;

    Argon2id(&context);

    assert(out[0] == 0x3E && out[8] == 0xBD && out[15] == 0x9D);
}

BENCHMARK(khovratovich, Argon2ds, 10, 1) {
    CREATE_CONTEXT;

    Argon2ds(&context);

    assert(out[0] == 0x6A && out[8] == 0x5E && out[15] == 0x84);
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(khovratovich, Argon2d); // Memory-hard scheme Argon2d
    REGISTER_BENCHMARK(khovratovich, Argon2i); // Memory-hard scheme Argon2i
    REGISTER_BENCHMARK(khovratovich, Argon2id); // Memory-hard scheme Argon2id
    REGISTER_BENCHMARK(khovratovich, Argon2ds); // Memory-hard scheme Argon2ds

    RUN_BENCHMARKS(argc, argv);

    return 0;
}
