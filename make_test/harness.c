#include <argp.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "harness.h"


static char doc[] = "Test Harness";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = {
    { "warmup", 'w', "COUNT", OPTION_ARG_OPTIONAL, "number of warmup iterations to do"},
    { "measure", 'm', "COUNT", OPTION_ARG_OPTIONAL, "number of measured iterations to do"},
    { "details", 'd', 0, 0, "show details about the test harness to execute"},
    { 0 }
};

struct arguments {
    int warmup_it;
    int measure_it;
    bool show_details;
};

static int print_details(_test_harness test_harness);
static long do_benchmark(_test_harness test_harness, int iterations);

static error_t parse_opt(int key, char *arg, struct argp_state *state);
static error_t read_unsigned_int_arg(int *var, char *arg);

int _execute_harness(int argc, char* argv[], _test_harness test_harness) {
    struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

    struct arguments arguments = {
        .warmup_it = 10,
        .measure_it = 10,
        .show_details = false
    };

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if(arguments.show_details || true) {
        /*return*/ print_details(test_harness);
    }

    printf(" * Warmup Iterations: %d\n", arguments.warmup_it);
    printf(" * Measure Iterations: %d\n", arguments.measure_it);
    printf("########################################\n");

    // Do benchmark
    int problem_size = arguments.measure_it;
    int warmup_iterations = arguments.warmup_it / problem_size;

    // Warmup
    if(arguments.warmup_it%problem_size) {
        do_benchmark(test_harness, arguments.warmup_it%problem_size);
    }
    for(int i=0; i < warmup_iterations; i++) {
        do_benchmark(test_harness, problem_size);
    }
    printf("\n");

    // Final run
    long result = do_benchmark(test_harness, arguments.measure_it);
    printf("\n");

    printf(" * execution time: %ldus\n", result);
    printf("########################################\n");

    return 0;
}

static int print_details(_test_harness harness) {
    printf("########################################\n");
    printf(" * Name: \"%s\"\n", harness.name);
    printf(" * Description: \"%s\"\n", harness.description);
    printf(" * Expected Iteration time: %.3fs\n", harness.expected_runtime/1000.);
    printf("########################################\n");

    return 0;
}

static long do_benchmark(_test_harness test_harness, int iterations) {
    struct timeval tv_start, tv_end;
    struct timeval *tv_start_ptr = &tv_start, *tv_end_ptr=&tv_end;

    long time = 0L;

    for(int i = 0; i < iterations; i++) {
        gettimeofday(tv_start_ptr, NULL);
        test_harness.test_harness(); // TODO: do not optimize away
        gettimeofday(tv_end_ptr, NULL);
        time += (tv_end.tv_sec-tv_start.tv_sec)*1000L+(long)((tv_end.tv_usec-tv_start.tv_usec)*0.001);
        printf(".");
        fflush(stdout);
    }

    return time /* /iterations */; // TODO
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'w':
            return read_unsigned_int_arg(&(arguments->warmup_it), arg+1);

        case 'm':
            return read_unsigned_int_arg(&(arguments->measure_it), arg+1);

        case 'd':
            arguments->show_details = true;
            return 0;

        case ARGP_KEY_ARG:
            return 0;

        default:
            return ARGP_ERR_UNKNOWN;
    }
}

static error_t read_unsigned_int_arg(int *var, char *arg) {
    char *endptr;
    errno = 0;
    int parsed = (int) strtol(arg, &endptr, 10);
    if (errno == ERANGE || *endptr != '\0' || arg == endptr || parsed < 0) {
        printf("Invalid number: \"%s\"\n", arg);
        return ARGP_ERR_UNKNOWN;
    }
    *var = parsed;
    return 0;
}

