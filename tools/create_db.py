#!/usr/bin/env python3

import argparse
import json
import logging
import sys

from dateutil.parser import parse as date_parse

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

logging.getLogger('sqlalchemy').setLevel(logging.WARNING)
logging.getLogger('sqlalchemy.engine.base.Engine').setLevel(logging.WARNING)
logging.getLogger('sqlalchemy.orm.mapper.Mapper').setLevel(logging.WARNING)

import util.datamodel as dm  # NOQA:E402
from util.color_logger import get_logger  # NOQA:E402

logger = get_logger('create_db')


def get_benchmark_name(benchmark):
    return "{}.{}".format(benchmark.get('fixture'), benchmark.get('name'))


def load_file_in_db(session, file):
    file_data = json.load(file)

    harness_data = {}
    if 'benchmark_data' in file_data:
        benchmark_data = file_data['benchmark_data']
        harness_data = file_data.get('harness_data', {})
    else:
        benchmark_data = file_data  # old file structure

    for runtime_data in benchmark_data.values():
        for runtime, data in runtime_data.items():
            if type(data) is not list:
                data = [data]  # old file structure

            run_id = 0
            for entry in data:
                entry_h_data = harness_data.get(entry['harness'], {}).get(runtime, []) if 'harness' in entry else []
                h_data = entry_h_data[run_id] if len(entry_h_data) > run_id else {}

                harness = session.query(dm.Harness).filter_by(name=entry['harness']).one_or_none()
                if harness is None:
                    harness = dm.Harness(name=entry['harness'])
                    session.add(harness)

                benchmark = session.query(dm.Benchmark).filter_by(name=get_benchmark_name(entry)).one_or_none()
                if benchmark is None:
                    benchmark = dm.Benchmark(name=get_benchmark_name(entry), harness=harness)
                    session.add(benchmark)

                config = session.query(dm.Configuration).filter_by(name=runtime, harness=harness).one_or_none()
                if config is None:
                    config = dm.Configuration(name=runtime, harness=harness)
                    session.add(config)

                # TODO: one Execution can contain multiple RUNS!
                h_system = h_data.get('system', {})
                h_cpu = h_system.get('cpu', {})
                h_memory = h_system.get('memory', {})

                execution = dm.Execution(configuration=config,
                                         datetime=date_parse(h_data['datetime']) if 'datetime' in h_data else None,
                                         stderr=h_data.get('stderr'),
                                         stdout=h_data.get('stdout'),
                                         exit_code=h_data.get('exit_code'),
                                         sys_platform=h_system.get('platform'),
                                         sys_mem_avail=h_memory.get('available'),
                                         sys_mem_free=h_memory.get('free'),
                                         sys_mem_total=h_memory.get('total'),
                                         sys_mem_used=h_memory.get('used'),
                                         sys_cpu_logical=h_cpu.get('cores_logical'),
                                         sys_cpu_physical=h_cpu.get('cores_physical'))
                session.add(execution)

                session.add_all([dm.ExecutionBuildSystem(execution=execution, key=key, value=value)
                                 for key, value in h_data.get('build_system', {}).items()])

                session.add_all([dm.ExecutionMakeEnv(execution=execution, key=key, value=value)
                                 for key, value in h_data.get('make_env', {}).items()])

                session.add_all([dm.ExecutionSystemCpu(execution=execution, idx=idx, percent=val[0],
                                                       cur_clock=val[1][0], min_clock=val[1][1], max_clock=val[1][2])
                                 for idx, val in enumerate(zip(h_cpu.get('percent', []), h_cpu.get('freq', [])))])

                run = dm.Run(execution=execution,
                             benchmark=benchmark,
                             clock_resolution=entry.get('clock_resolution'),
                             clock_resolution_measured=entry.get('clock_resolution_measured'),
                             clock_type=entry.get('clock_type'),
                             disabled=entry.get('disabled'),
                             iterations_per_run=entry.get('iterations_per_run'))

                for idx, dp in enumerate(entry['runs']):
                    session.add_all([dm.Datapoint(idx=idx, run=run, key=key, value=value) for key, value in dp.items()])

                session.commit()

                run_id += 1


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
    dm.Base.metadata.bind = engine
    dm.Base.metadata.create_all(engine)

    DBSession = sessionmaker()
    DBSession.bind = engine

    session = DBSession()

    for file in args.infile:
        try:
            load_file_in_db(session, file)
        except Exception:
            logger.exception("cannot load file")
            sys.exit()
