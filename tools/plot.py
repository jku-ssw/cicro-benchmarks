#!/usr/bin/env python3

import argparse
import logging
import sys

import matplotlib.pyplot as plt

from util.bench_results import BenchmarkingResults
from util.color_logger import get_logger
from util.analyze import preprocess

logger = get_logger('plot')


def boxplot(datapoints):
    # get all valid labels
    labels = set()
    for value in datapoints.values():
        labels.update(value.keys())
    labels = sorted(labels, reverse=True)

    # creaet value arrays
    values = []
    for label in labels:
        label_values = []
        for value in datapoints.values():
            if label in value:
                assert value[label]['duration']['mean'] != 0
                label_values.append(value[label]['duration']['mean'])
        values.append(label_values)

    fig = plt.figure()
    ax = fig.add_subplot(111)

    ax.boxplot(values, vert=False, notch=1, labels=labels)

    plt.show()


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

    if not processed_data:
        logger.error('no data to plot!')
        sys.exit()

    logger.debug(str(processed_data))

    boxplot(processed_data)
