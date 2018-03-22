#include <argp.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "harness.h"

#define OVERRIDE_STD_CALLS

#ifdef OVERRIDE_STD_CALLS
// override printf function
int printf (const char * format, ...) {
    return 0;
}

#define printf(format, ...) fprintf (stdout, format, __VA_ARGS__)

#endif



static char doc[] = "Test Harness";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = {
    { "warmup_i", 'w', "COUNT", OPTION_ARG_OPTIONAL, "minimal number of warmup iterations to do"},
    { "measure_i", 'm', "COUNT", OPTION_ARG_OPTIONAL, "minimal number of measurement iterations to do"},
    { "warmup_t", 'x', "COUNT", OPTION_ARG_OPTIONAL, "minimal time to do warmup (in milliseconds)"},
    { "measure_t", 'y', "COUNT", OPTION_ARG_OPTIONAL, "minimal time to do measurements (in milliseconds)"},
    { "details", 'd', 0, 0, "show details about the test harness to execute"},
    { 0 }
};

struct arguments {
    int warmup_it;
    int measure_it;
    int warmup_time;
    int measure_time;
    bool show_details;
};

static int print_details(_test_harness test_harness);
static double do_benchmark(_test_harness test_harness, int min_iterations, double min_time);

double getCPUTime();

static error_t parse_opt(int key, char *arg, struct argp_state *state);
static error_t read_unsigned_int_arg(int *var, char *arg);

int _execute_harness(int argc, char* argv[], _test_harness test_harness) {
    struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

    struct arguments arguments = {
        .warmup_it = 0,
        .measure_it = 0,
        .warmup_time = 0,
        .measure_time = 0,
        .show_details = false
    };

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // Load default values if nothing was specified
    if(arguments.warmup_it == 0&& arguments.warmup_time == 0) {
        arguments.warmup_it = 10;
    }
    if(arguments.measure_it == 0 && arguments.measure_time == 0) {
        arguments.measure_it = 10;
    }

    if(arguments.show_details) {
        print_details(test_harness);

        printf(" * Warmup Iterations: %d\n", arguments.warmup_it);
        printf(" * Measure Iterations: %d\n", arguments.measure_it);
        printf(" * Warmup Time: %d\n", arguments.warmup_time);
        printf(" * Measure Time: %d\n", arguments.measure_time);
        puts("########################################");
    }

    // Warmup
    do_benchmark(test_harness, arguments.warmup_it, ((double)arguments.warmup_time)/1000.);
    putchar('\n');

    // Final run
    double result = do_benchmark(test_harness, arguments.measure_it, ((double)arguments.measure_time)/1000.);
    putchar('\n');

    printf(" * execution time: %fms\n", result);
    puts("########################################");

    return 0;
}

static int print_details(_test_harness harness) {
    puts("########################################");
    printf(" * Name: \"%s\"\n", harness.name);
    printf(" * Description: \"%s\"\n", harness.description);
    printf(" * Expected Iteration time: %.3fs\n", harness.expected_runtime/1000.);
    puts("########################################");

    return 0;
}

static double do_benchmark(_test_harness test_harness, int min_iterations, double min_time) {
    double startTime, endTime;
    double sumTime = 0;

    int it = 0;

    for(; it < min_iterations || sumTime < min_time; it++) {
        startTime = getCPUTime();

        test_harness.test_harness(); // TODO: do not optimize away

        endTime = getCPUTime();
        sumTime += endTime - startTime;

        putchar('.');
        fflush(stdout);
    }

    return sumTime / (double)it;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = (struct arguments *) state->input;
    switch (key) {
        case 'w':
            return read_unsigned_int_arg(&(arguments->warmup_it), arg);

        case 'm':
            return read_unsigned_int_arg(&(arguments->measure_it), arg);

        case 'x':
            return read_unsigned_int_arg(&(arguments->warmup_time), arg);

        case 'y':
            return read_unsigned_int_arg(&(arguments->measure_time), arg);

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
    if(arg == NULL) {
        return ARGP_ERR_UNKNOWN;
    }
    char *endptr;
    errno = 0;
    int parsed = (int) strtol(arg+1, &endptr, 10);
    if (errno == ERANGE || *endptr != '\0' || arg+1 == endptr || parsed < 0) {
        printf("Invalid number: \"%s\"\n", arg+1);
        return ARGP_ERR_UNKNOWN;
    }
    *var = parsed;
    return 0;
}

#include <sys/times.h>
#include <unistd.h>
#include <time.h>

/**
 * Returns the amount of CPU time used by the current process,
 * in seconds, or -1.0 if an error occurred.
 *
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 *
 * See: http://nadeausoftware.com/articles/2012/03/c_c_tip_how_measure_cpu_time_benchmarking
 */
double getCPUTime( )
{

#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
	/* Prefer high-res POSIX timers, when available. */
	{
		clockid_t id;
		struct timespec ts;
#if _POSIX_CPUTIME > 0
		/* Clock ids vary by OS.  Query the id, if possible. */
		if ( clock_getcpuclockid( 0, &id ) == -1 )
#endif
#if defined(CLOCK_PROCESS_CPUTIME_ID)
			/* Use known clock id for AIX, Linux, or Solaris. */
			id = CLOCK_PROCESS_CPUTIME_ID;
#elif defined(CLOCK_VIRTUAL)
			/* Use known clock id for BSD or HP-UX. */
			id = CLOCK_VIRTUAL;
#else
			id = (clockid_t)-1;
#endif
		if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
			return (double)ts.tv_sec +
				(double)ts.tv_nsec / 1000000000.0;
	}
#endif

#if defined(RUSAGE_SELF)
	{
		struct rusage rusage;
		if ( getrusage( RUSAGE_SELF, &rusage ) != -1 )
			return (double)rusage.ru_utime.tv_sec +
				(double)rusage.ru_utime.tv_usec / 1000000.0;
	}
#endif

	{
		const double ticks = (double)sysconf( _SC_CLK_TCK );
		struct tms tms;
		if ( times( &tms ) != (clock_t)-1 )
			return (double)tms.tms_utime / ticks;
	}

    return -1;		/* Failed. */
}