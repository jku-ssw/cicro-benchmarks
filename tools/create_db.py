#!/usr/bin/env python3

import argparse
import logging
import sys

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

logging.getLogger('sqlalchemy').setLevel(logging.WARNING)
logging.getLogger('sqlalchemy.engine.base.Engine').setLevel(logging.WARNING)
logging.getLogger('sqlalchemy.orm.mapper.Mapper').setLevel(logging.WARNING)

from util.datamodel import Base  # NOQA:E402
from util.color_logger import get_logger  # NOQA:E402
from util.datamodel_helper import load_file_in_db  # NOQA:E402

logger = get_logger('create_db')


if __name__ == "__main__":
    # Parse Command-line Arguments
    parser = argparse.ArgumentParser(description='Export benchmark results into sql database',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog,
                                                                                         max_help_position=30,
                                                                                         width=120))

    parser.add_argument('infile', metavar='INFILE', nargs="+", type=argparse.FileType('r'),
                        help='file where the benchmarks are located')
    parser.add_argument('database', metavar='OUTFILE', type=str,
                        help='file where we want to store the benchcmarks (sqllite)')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    engine = create_engine('sqlite:///{}'.format(args.database))
    Base.metadata.bind = engine
    Base.metadata.create_all(engine)

    DBSession = sessionmaker()
    DBSession.bind = engine

    session = DBSession()

    for file in args.infile:
        try:
            load_file_in_db(session, file)
        except Exception:
            logger.exception("cannot load file")
            sys.exit()
