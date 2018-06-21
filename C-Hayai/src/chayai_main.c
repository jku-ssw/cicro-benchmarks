
#include "chayai_console.h"
#include "chayai_console_outputter.h"
#include "chayai_json_outputter.h"
#include "chayai_main.h"

#include "stddef.h"
#include <string.h>
#include <stdlib.h>
#include <argp.h>

#ifdef USE_PAPI
#include <papi.h>
#endif

#define OUTPUT_STREAM stdout

#if defined(_WIN32)
#    define PATH_SEPARATOR '\\'
#else
#    define PATH_SEPARATOR '/'
#endif


static char chayai_doc[] = "Runs the benchmarks for this project";

static char chayai_args_doc[] = "[OPTIONS]";

static struct argp_option chayai_options[] = {
        { "output", 'o', "console|json", OPTION_ARG_OPTIONAL, "Output results in a specific format."},
        { "warmup", 'w', "2", OPTION_ARG_OPTIONAL, "Number of warmup iterations"},
        { "iterations", 'i', "0", OPTION_ARG_OPTIONAL, "Number of benchmark iterations"},
#ifdef USE_PAPI
        { "papi", 'p', "PAPI_TOT_CYC,PAPI_TOT_INS", OPTION_ARG_OPTIONAL, "Papi events to catch"},
#endif
        { 0 }
};

static error_t read_unsigned_int_arg(unsigned int *var, char *arg) {
    if(arg == NULL) {
        return ARGP_ERR_UNKNOWN;
    }
    char *endptr;
    errno = 0;
    unsigned int parsed = (unsigned int) strtol(arg, &endptr, 10);
    if (errno == ERANGE || *endptr != '\0' || arg == endptr || parsed < 0) {
        fprintf(stderr, "Invalid number: \"%s\" (%s:%d)\n", arg, __FILE__, __LINE__);
        return ARGP_ERR_UNKNOWN;
    }
    *var = parsed;
    return 0;
}

static error_t chayai_parse_opt(int key, char *arg, struct argp_state *state) {
    CHayaiArguments *arguments = (CHayaiArguments*) state->input;
    switch (key) {
        case 'o':
            if(strcmp(arg,"console") == 0) {
                arguments->outputterInit = chayai_console_outputter_init;
                return 0;
            } else if(strcmp(arg,"json") == 0) {
                arguments->outputterInit = chayai_json_outputter_init;
                return 0;
            } else {
                return ARGP_ERR_UNKNOWN;  // TODO: invalid error message
            }

        case 'w':
            return read_unsigned_int_arg(&(arguments->warmupIterations), arg);

        case 'i':
            return read_unsigned_int_arg(&(arguments->benchmarkIterations), arg);

#ifdef USE_PAPI
        case 'p': {
            if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
                fprintf(stderr, "PAPI init error! (%s:%d)\n", __FILE__, __LINE__);
                exit(2);
            }

            int retval;
            if (arguments->papiEventSet == PAPI_NULL && (retval = PAPI_create_eventset(&(arguments->papiEventSet))) != PAPI_OK) {
                fprintf(stderr, "PAPI error %d: %s (%s:%d)\n", retval, PAPI_strerror(retval), __FILE__, __LINE__);
                exit(2);
            }

            char *token;
            while ((token = strsep(&arg, ","))) {
                if((retval = PAPI_add_named_event(arguments->papiEventSet, token)) != PAPI_OK) {
                    fprintf(stderr, "PAPI error %d for \"%s\": %s (%s:%d)\n", retval, token, PAPI_strerror(retval), __FILE__, __LINE__);
                    exit(2);
                }
            }
        }
            return 0;
#endif

        case ARGP_KEY_ARG:
            return 0;

        default:
            return ARGP_ERR_UNKNOWN;
    }
}


CHayaiArguments chayai_main_parse_args(int argv, char** argc) {
    struct argp argp = {chayai_options, chayai_parse_opt, chayai_args_doc, chayai_doc, 0, 0, 0 };

    CHayaiArguments arguments = {
        .outputterInit = chayai_console_outputter_init,
        .warmupIterations = 2, // to do at least some caching by default before running the benchmark
        .benchmarkIterations = 0, // 0 means use number of iterations given by benchmark
#ifdef USE_PAPI
        .papiEventSet = PAPI_NULL,
#endif
    };

    argp_parse(&argp, argv, argc, 0, 0, &arguments);

    return arguments;
}

void chayai_main_cleanup_arguments(CHayaiArguments *arguments) {
#ifdef USE_PAPI
    if(arguments->papiEventSet != PAPI_NULL) {
        int retval;
        if((retval = PAPI_cleanup_eventset(arguments->papiEventSet)) != PAPI_OK) {
            fprintf(stderr, "PAPI error %d: %s (%s:%d)\n", retval, PAPI_strerror(retval), __FILE__, __LINE__);
            return;
        }
        if((retval = PAPI_destroy_eventset(&arguments->papiEventSet)) != PAPI_OK) {
            fprintf(stderr, "PAPI error %d: %s (%s:%d)\n", retval, PAPI_strerror(retval), __FILE__, __LINE__);
        }

        PAPI_shutdown();
    }
#endif
}

// TODO: unused
static void chayai_main_show_usage(char* execName) {
    const char* baseName = strrchr(execName, PATH_SEPARATOR);

    fprintf(OUTPUT_STREAM, "Usage: %s ", baseName ? baseName + 1 : execName);
    chayai_console_change_color(OUTPUT_STREAM, CHAYAI_TEXT_GREEN);
    fputs("[OPTIONS]", OUTPUT_STREAM);
    chayai_console_change_color(OUTPUT_STREAM, CHAYAI_TEXT_DEFAULT);
    fputs("\n\n", OUTPUT_STREAM);

    fputs("  Runs the benchmarks for this project.\n\n", OUTPUT_STREAM);

    fputs("Miscellaneous options:\n", OUTPUT_STREAM);
    fputs("  ", OUTPUT_STREAM);
    chayai_console_change_color(OUTPUT_STREAM, CHAYAI_TEXT_GREEN);
    fputs("-?", OUTPUT_STREAM);
    chayai_console_change_color(OUTPUT_STREAM, CHAYAI_TEXT_DEFAULT);
    fputs(", ", OUTPUT_STREAM);
    chayai_console_change_color(OUTPUT_STREAM, CHAYAI_TEXT_GREEN);
    fputs("-h", OUTPUT_STREAM);
    chayai_console_change_color(OUTPUT_STREAM, CHAYAI_TEXT_DEFAULT);
    fputs(", ", OUTPUT_STREAM);
    chayai_console_change_color(OUTPUT_STREAM, CHAYAI_TEXT_GREEN);
    fputs("--help", OUTPUT_STREAM);
    chayai_console_change_color(OUTPUT_STREAM, CHAYAI_TEXT_DEFAULT);
    fputs("\n    Show this help information.\n\n", OUTPUT_STREAM);
}