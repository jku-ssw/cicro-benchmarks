#ifndef JKU_TEST_HARNESS
#define JKU_TEST_HARNESS

struct __test_harness {
    char* name;
    int (*test_harness)(void);
    int expected_runtime;
};

typedef struct __test_harness __test_harness;

void __execute_harness(int argv, char** argc, __test_harness test_harness);

#endif // JKU_TEST_HARNESS
