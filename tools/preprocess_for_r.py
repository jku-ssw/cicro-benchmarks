#!/usr/bin/env python3

import argparse
import json
import logging
import sys

from util.bench_results import BenchmarkingResults
from util.color_logger import get_logger
from util.analyze import preprocess

logger = get_logger('preprocess')


if __name__ == "__main__":
    # Parse Command-line Arguments
    parser = argparse.ArgumentParser(description='Plot benchmark results',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog,
                                                                                         max_help_position=30,
                                                                                         width=120))

    parser.add_argument('benchfile', metavar='BENCHFILE', type=argparse.FileType('r'),
                        help='file where the benchmarks are written to')
    parser.add_argument('base', metavar='BASE_RUNTIME', type=str,
                        help='runtime run which is used as base')

    parser.add_argument('--filter-runtime', metavar='REGEX', type=str, default='.*',
                        help='regular expression to select which runtimes should be used')
    parser.add_argument('--filter-benchmark', metavar='REGEX', type=str, default='.*',
                        help='regular expression to select which harness should be used (based on benchmark name)')

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

    if args.base not in results.get_all_runtimes():
        logger.error('%s is not a valid runtime', args.base)
        logger.info('valid: %s', results.get_all_runtimes())
        sys.exit()

    processed_data = preprocess(results,
                                args.base,
                                filter_runtime=args.filter_runtime,
                                filter_benchmark=args.filter_benchmark)

    # combine datapoints to sinple metrics which we can use for boxplots
    if not processed_data:
        logger.error('no data to output!')
        sys.exit()

    logger.debug(processed_data)

    # find out which types can be plotted
    types = set()
    runtimes = set()
    for benchmark in processed_data.values():
        for runtime, values in benchmark.items():
            runtimes.add(runtime)
            types.update(values.keys())

    output = {}
    for t in types:
        output[t] = {}
        for r in runtimes:
            output[t][r] = {}
            for benchmark, data in processed_data.items():
                if benchmark not in processed_data:
                    continue
                if r not in processed_data[benchmark]:
                    continue
                if t not in processed_data[benchmark][r]:
                    continue

                output[t][r][benchmark] = processed_data[benchmark][r][t]['mean']

    print(json.dumps(output))
