#!/usr/bin/env python3

import argparse
import logging
import sys

from util.bench_results import BenchmarkingResults
from util.color_logger import get_logger


logger = get_logger('stats' )


def log_general_stats(results):
    logger.info("number of different benchmarks present: %d", len(results.get_all_benchmark_names()))

    for runtime in sorted(results.get_all_runtimes()):
        logger.info('runtime "%s" has %d benchmarks', runtime, len(list(results.get_all_benchmarks_of_runtime(runtime))))

        missing_benchmarks = results.get_missing_benchmark_names(runtime)
        if missing_benchmarks:
            logger.warning('  missing: %s', missing_benchmarks)


if __name__ == "__main__":
    # Parse Command-line Arguments
    parser = argparse.ArgumentParser(description='Evaluate benchmark results',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=30, width=120))

    parser.add_argument('benchfile',metavar='BENCHFILE', type=argparse.FileType('r'),
                        help='file where the benchmarks are written to')

    parser.add_argument('--filter-runtimes', metavar='REGEX', type=str, default='gcc-O0|clang-O0',
                        help='regular expression to select which runtimes should be used')
    parser.add_argument('--filter-harness', metavar='REGEX', type=str, default='.*',
                        help='regular expression to select which harness should be used (based on filename)')

    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    results = BenchmarkingResults()

    try:
        results.load_file(args.benchfile)
    except Exception:
        logger.exception("cannot load file")
        sys.exit()

    log_general_stats(results)