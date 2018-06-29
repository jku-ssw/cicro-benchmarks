#!/usr/bin/env python3

import argparse
import logging
import sys

from util.bench_results import BenchmarkingResults
from util.color_logger import get_logger


logger = get_logger('merge_results')


if __name__ == "__main__":
    # Parse Command-line Arguments
    parser = argparse.ArgumentParser(description='Merge benchmark results into one file',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog,
                                                                                         max_help_position=30,
                                                                                         width=120))

    parser.add_argument('infile', metavar='INFILE', nargs="+", type=argparse.FileType('r'),
                        help='file where the benchmarks are merged into')
    parser.add_argument('outfile', metavar='OUTFILE', type=argparse.FileType('x'),
                        help='file where the benchmarks are merged into')

    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    results = BenchmarkingResults()

    for file in args.infile:
        try:
            results.load_file(file, append=False, verbose=False)
        except Exception:
            logger.exception("cannot load file")
            sys.exit()

    results.store_file(args.outfile)
