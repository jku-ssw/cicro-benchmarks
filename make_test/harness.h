#ifndef JKU_TEST_HARNESS
#define JKU_TEST_HARNESS

struct _test_harness {
    char* name;
    char* description;
    int  (*test_harness)(void);
    long expected_runtime;
};

typedef struct _test_harness _test_harness;

int _execute_harness(int argc, char* argv[], _test_harness test_harness);

#endif // JKU_TEST_HARNESS
