#!/usr/bin/env python3

import argparse
import logging
import sys

from util.color_logger import get_logger
from util.datamodel_helper import create_db_session, load_file_in_db, save_file_as_json

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
    parser.add_argument('--database', default='sqlite://', type=str,
                        help='database which is used for procecssing. By default use in-memory sqllite')
    parser.add_argument('--filter-runtime', metavar='REGEX', type=str, default='.*',
                        help='regular expression to select which runtimes should be used')

    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    session = create_db_session(args.database)

    for file in args.infile:
        try:
            logger.info("load \"{}\"".format(file.name))
            load_file_in_db(session, file)
        except Exception:
            logger.exception("cannot load file")
            sys.exit()

    logger.info("store into \"{}\"".format(args.outfile.name))
    save_file_as_json(session, args.outfile, args.filter_runtime)
