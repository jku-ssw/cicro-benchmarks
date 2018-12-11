#!/usr/bin/env python3

import argparse
import csv
import logging
import sys
import re

from sqlalchemy.orm import selectinload

from util.color_logger import get_logger
import util.datamodel as dm
from util.datamodel_helper import create_db_session, load_file_in_db

logger = get_logger('extract_gcov')

CSV_BENCHMARK_NAME = 'benchmark name'
CSV_FILE_NAME = 'file name'
CSV_EXECUTED_LOC = 'executed LOC'
CSV_TOTAL_LOC = 'total LOC'


def parse_gcov_output(benchmark_name, output):
    lines = output.splitlines()
    for i in range(0, len(lines), 3):
        file_matcher = re.match("File '(.*)'", lines[i])
        loc_matcher = re.match("Lines executed:(.*)% of ([0-9]*)", lines[i+1])
        file_name = file_matcher.group(1)
        if loc_matcher is None:
            assert(lines[i+1] == 'No executable lines')
            percentage_executed = 0.0
            total_loc = 0
        else:
            percentage_executed = float(loc_matcher.group(1))/100
            total_loc = int(loc_matcher.group(2))
        executed_loc = int(percentage_executed * total_loc)

        yield {
            CSV_BENCHMARK_NAME: benchmark_name,
            CSV_FILE_NAME: file_name,
            CSV_EXECUTED_LOC: executed_loc,
            CSV_TOTAL_LOC: total_loc
        }


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Merge benchmark results into one file',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog,
                                                                                         max_help_position=30,
                                                                                         width=120))

    parser.add_argument('infile', metavar='INFILE', nargs="+", type=argparse.FileType('r'),
                        help='file where the benchmarks are merged into')
    parser.add_argument('--filter-runtime', metavar='REGEX', type=str, default='gcc-gcov-line-numbers.*',
                        help='regular expression to select which runtimes should be used')

    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    session = create_db_session()

    for file in args.infile:
        try:
            load_file_in_db(session, file)
        except Exception:
            logger.exception("cannot load file")
            sys.exit()

    fieldnames = [CSV_BENCHMARK_NAME, CSV_FILE_NAME, CSV_EXECUTED_LOC, CSV_TOTAL_LOC]
    writer = csv.DictWriter(sys.stdout, fieldnames=fieldnames, delimiter=';')

    # store all execution
    writer.writeheader()
    for execution in session.query(dm.Execution).options(selectinload(dm.Execution.configuration)).all():
        config_name = execution.configuration.name
        harness_name = execution.harness.name

        if not re.match(args.filter_runtime, config_name):
            logger.debug('"{}" does not match filter -> skip'.format(config_name))
            continue

        if execution.stderr is None:
            logger.debug('execution "{}" has empty stderr -> skip'.format(execution.id))
            continue

        parsed_gcov_output = parse_gcov_output(execution.harness.name, execution.stderr)
        for line in parsed_gcov_output:
            writer.writerow(line)
