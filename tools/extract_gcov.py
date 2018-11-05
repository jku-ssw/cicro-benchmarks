#!/usr/bin/env python3

import argparse
import logging
import sys
import re

from util.bench_results import BenchmarkingResults
from util.color_logger import get_logger


logger = get_logger('extract_gcov')

def parse_gcov_output(benchmark_name, output):
    lines = output.splitlines()
    result_lines = []
    for i in range(0, len(lines), 3):
        file_matcher = re.match("File '(.*)'", lines[i])
        loc_matcher = re.match("Lines executed:(.*)% of ([0-9]*)", lines[i+1])
        file_name = file_matcher.group(1)
        if loc_matcher is None:
            assert(lines[i+1] == 'No executable lines')
            percentage_executed = 0.0
            total_loc = 0
        else:
            percentage_executed = float(loc_matcher.group(1))
            total_loc = int(loc_matcher.group(2))
        executed_loc = int(percentage_executed/100 * total_loc)
        result_lines.append('%s;%s;%d;%d' % (benchmark_name, file_name, executed_loc, total_loc))
    return result_lines

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Merge benchmark results into one file',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog,
                                                                                         max_help_position=30,
                                                                                         width=120))

    parser.add_argument('infile', metavar='INFILE', nargs="+", type=argparse.FileType('r'),
                        help='file where the benchmarks are merged into')
    parser.add_argument('--filter-runtime', metavar='REGEX', type=str, default='.*',
                        help='regular expression to select which runtimes should be used')

    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    results = BenchmarkingResults()

    for file in args.infile:
        try:
            print('benchmark name;file name;executed LOC;total LOC')
            results.load_file(file, append=False, verbose=False)
            data = results.get_all()
            for datum in data:
                    harness_data = datum['harness_data']
                    if 'stderr' in harness_data:
                        if args.filter_runtime and datum['runtime'] == args.filter_runtime:
                            gcov_output = harness_data['stderr']
                            parsed_gcov_output = parse_gcov_output(datum['name'], gcov_output)
                            for line in parsed_gcov_output:
                                print(line)
        except Exception:
            logger.exception("cannot load file")
            sys.exit()
