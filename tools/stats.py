#!/usr/bin/env python3

import argparse
import logging
import sys

from sqlalchemy import create_engine, func
from sqlalchemy.orm import sessionmaker

from util.color_logger import get_logger

logging.getLogger('sqlalchemy').setLevel(logging.WARNING)
logging.getLogger('sqlalchemy.engine.base.Engine').setLevel(logging.WARNING)
logging.getLogger('sqlalchemy.orm.mapper.Mapper').setLevel(logging.WARNING)

import util.datamodel as dm  # NOQA:E402
from util.datamodel_helper import load_file_in_db  # NOQA:E402

logger = get_logger('stats')


def log_general_stats(session):
    logger.info("number of different benchmarks present: %d", session.query(dm.Benchmark).group_by('name').count())

    for config in session.query(dm.Configuration).all():
        num_of_benchmarks = (session.query(func.count(dm.Benchmark.name.distinct()))
                             .join(dm.Harness, dm.Benchmark.harness_id == dm.Harness.id)
                             .join(dm.Execution, dm.Harness.id == dm.Execution.harness_id)
                             .join(dm.Configuration, dm.Execution.configuration_id == dm.Configuration.id)
                             .filter(dm.Configuration.name == config.name)
                             .group_by(dm.Configuration.name)
                             ).scalar()

        logger.info('runtime "%s" has %d benchmarks', config.name, num_of_benchmarks)

        query_incl_benchmarks = (session.query(dm.Benchmark.id.distinct())
                                 .join(dm.Harness, dm.Benchmark.harness_id == dm.Harness.id)
                                 .join(dm.Execution, dm.Harness.id == dm.Execution.harness_id)
                                 .join(dm.Configuration, dm.Execution.configuration_id == dm.Configuration.id)
                                 .filter(dm.Configuration.name == config.name)
                                 )

        missing_benchmarks = session.query(dm.Benchmark.name).filter(~dm.Benchmark.id.in_(query_incl_benchmarks)).all()

        if missing_benchmarks:
            logger.warning('  missing: %s', [b[0] for b in missing_benchmarks])


if __name__ == "__main__":
    # Parse Command-line Arguments
    parser = argparse.ArgumentParser(description='Evaluate benchmark results',
                                     formatter_class=lambda prog: argparse.HelpFormatter(prog,
                                                                                         max_help_position=30,
                                                                                         width=120))

    parser.add_argument('benchfile', metavar='BENCHFILE', type=argparse.FileType('r'),
                        help='file where the benchmarks are written to')

    # parser.add_argument('--filter-runtime', metavar='REGEX', type=str, default='gcc-O0|clang-O0',
    #                    help='regular expression to select which runtimes should be used')
    # parser.add_argument('--filter-harness', metavar='REGEX', type=str, default='.*',
    #                    help='regular expression to select which harness should be used (based on filename)')

    parser.add_argument('--verbose', '-v', action='store_true',
                        help='enable debug output')

    args = parser.parse_args()

    if not args.verbose:
        logging.disable(logging.DEBUG)  # we want to set all loggers

    engine = create_engine('sqlite://')
    dm.Base.metadata.bind = engine
    dm.Base.metadata.create_all(engine)

    DBSession = sessionmaker()
    DBSession.bind = engine

    session = DBSession()

    try:
        logger.info("load file")
        load_file_in_db(session, args.benchfile)
    except Exception:
        logger.exception("cannot load file")
        sys.exit()

    log_general_stats(session)
