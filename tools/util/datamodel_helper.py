import json
import re

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, joinedload

from dateutil.parser import parse as date_parse

import util.datamodel as dm
from util.color_logger import get_logger
from util.auto_extend_list import auto_extend_list

from datetime import timezone

logger = get_logger('datamodel_helper')


def create_db_session(db_engine='sqlite://'):
    logger.debug('create database session. Engine: "{}"'.format(db_engine))
    engine = create_engine(db_engine)
    dm.Base.metadata.bind = engine
    dm.Base.metadata.create_all(engine)

    DBSession = sessionmaker()
    DBSession.bind = engine

    return DBSession()


def get_benchmark_name(benchmark):
    fixture = benchmark.get('fixture')
    name = benchmark.get('name')
    assert ('.' not in fixture) and ('.' not in name)
    return "{}.{}".format(fixture, name)


def split_benchmark_name(benchmark_name):
    splits = benchmark_name.split('.')
    assert len(splits) == 2
    return splits  # fixure, name


def get_or_create_harness(session, harness_name):
    harness = session.query(dm.Harness).filter_by(name=harness_name).one_or_none()
    if harness is None:
        harness = dm.Harness(name=harness_name)
        session.add(harness)
    return harness


def get_or_create_benchmark(session, benchmark_name, harness):
    benchmark = session.query(dm.Benchmark).filter_by(name=benchmark_name).one_or_none()
    if benchmark is None:
        benchmark = dm.Benchmark(name=benchmark_name, harness=harness)
        session.add(benchmark)
    assert benchmark.harness == harness
    return benchmark


def get_or_create_config(session, config_name):
    config = session.query(dm.Configuration).filter_by(name=config_name).one_or_none()
    if config is None:
        config = dm.Configuration(name=config_name)
        session.add(config)
    return config


class DatabaseCache(object):
    def __init__(self, session):
        self.session = session

        self.configuration_cache = {}
        self.harness_cache = {}
        self.benchmark_cache = {}

        self._load_configuration_cache()
        self._load_harness_cache()
        self._load_benchmark_cache()

    def _load_configuration_cache(self):
        for config in self.session.query(dm.Configuration).all():
            self.configuration_cache[config.name] = config

    def _load_harness_cache(self):
        for harness in self.session.query(dm.Harness).all():
            self.harness_cache[harness.name] = harness

    def _load_benchmark_cache(self):
        for benchmark in self.session.query(dm.Benchmark).all():
            self.benchmark_cache[benchmark.name] = benchmark

    def get_or_create_config(self, config_name):
        if config_name in self.configuration_cache:
            return self.configuration_cache[config_name]
        else:
            config = get_or_create_config(self.session, config_name)
            self.configuration_cache[config_name] = config
            return config

    def get_or_create_harness(self, harness_name):
        if harness_name in self.harness_cache:
            return self.harness_cache[harness_name]
        else:
            harness = get_or_create_harness(self.session, harness_name)
            self.harness_cache[harness_name] = harness
            return harness

    def get_or_create_benchmark(self, benchmark_name, harness):
        if benchmark_name in self.benchmark_cache:
            return self.benchmark_cache[benchmark_name]
        else:
            benchmark = get_or_create_benchmark(self.session, benchmark_name, harness)
            self.benchmark_cache[benchmark_name] = benchmark
            return benchmark


def load_file_in_db(session, file):
    cache = DatabaseCache(session)

    file_data = json.load(file)

    harness_data = {}
    if 'benchmark_data' in file_data:
        benchmark_data = file_data['benchmark_data']
        harness_data = file_data.get('harness_data', {})
    else:
        benchmark_data = file_data  # old file structure

    execution_cache = {}

    for runtime_data in benchmark_data.values():
        for runtime, data in runtime_data.items():
            if type(data) is not list:
                data = [data]  # old file structure

            config = cache.get_or_create_config(runtime)

            run_id = 0
            for entry in data:
                entry_h_data = harness_data.get(entry['harness'], {}).get(runtime, []) if 'harness' in entry else []
                h_data = entry_h_data[run_id] if len(entry_h_data) > run_id else {}

                harness = cache.get_or_create_harness(entry['harness'])
                benchmark = cache.get_or_create_benchmark(get_benchmark_name(entry), harness)

                h_system = h_data.get('system', {})
                h_cpu = h_system.get('cpu', {})
                h_memory = h_system.get('memory', {})

                execution_cache_arr = execution_cache.get(harness.name, {}).get(runtime, [])
                if run_id < len(execution_cache_arr) and execution_cache_arr[run_id] is not None:
                    execution = execution_cache[harness.name][runtime][run_id]
                else:
                    # store datetime in UTC
                    dt = date_parse(h_data['datetime']).astimezone(timezone.utc) if 'datetime' in h_data else None
                    execution = dm.Execution(configuration=config,
                                             harness=harness,
                                             datetime=dt,
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

                    if 'build_system' in h_data or 'make_env' in h_data:
                        compilation = dm.Compilation()  # TODO: clean step is not stored in file
                        execution.compilation = compilation
                        session.add(compilation)

                        session.add_all([dm.CompilationBuildSystem(compilation=compilation, key=key, value=value)
                                         for key, value in h_data.get('build_system', {}).items()])

                        session.add_all([dm.CompilationMakeEnv(compilation=compilation, key=key, value=value)
                                         for key, value in h_data.get('make_env', {}).items()])

                    session.add(execution)
                    session.add_all([dm.ExecutionSystemCpu(execution=execution, idx=idx, percent=val[0],
                                                           cur_clock=val[1][0], min_clock=val[1][1],
                                                           max_clock=val[1][2])
                                     for idx, val in
                                     enumerate(zip(h_cpu.get('percent', []), h_cpu.get('freq', [])))])

                    # store in execution cache
                    if harness.name not in execution_cache:
                        execution_cache[harness.name] = {}
                    if runtime not in execution_cache[harness.name]:
                        execution_cache[harness.name][runtime] = auto_extend_list(None)
                    execution_cache[harness.name][runtime][run_id] = execution

                run = dm.Run(execution=execution,
                             benchmark=benchmark,
                             clock_resolution=entry.get('clock_resolution'),
                             clock_resolution_measured=entry.get('clock_resolution_measured'),
                             clock_type=entry.get('clock_type'),
                             disabled=entry.get('disabled'),
                             iterations_per_run=entry.get('iterations_per_run'))
                session.add(run)

                datapoints = []
                for idx, dp in enumerate(entry['runs']):
                    datapoints += [dm.Datapoint(idx=idx, run=run, key=key, value=value)
                                   for key, value in dp.items()]
                session.add_all(datapoints)

                run_id += 1

    session.commit()
    return True


def save_file_as_json(session, file, runtime_filter='.*'):
    benchmark_data = {}
    harness_data = {}

    # store all executions
    for execution in session.query(dm.Execution).options(joinedload(dm.Execution.configuration)).all():
        config_name = execution.configuration.name
        harness_name = execution.harness.name

        if not re.match(runtime_filter, config_name):
            logger.debug('"{}" does not match filter -> skip'.format(config_name))
            continue

        exec_harness = {}

        # key: 'build_system'
        if execution.compilation:
            build_system = {}
            for entry in execution.compilation.build_system:
                build_system[entry.key] = entry.value
            if len(build_system) > 0:
                exec_harness['build_system'] = build_system

        # key: 'datetime'
        if execution.datetime is not None:
            exec_harness['datetime'] = execution.datetime.replace(tzinfo=timezone.utc).isoformat()

        # key: 'stderr'
        if execution.stderr is not None:
            exec_harness['stderr'] = execution.stderr

        # key: 'stdout'
        if execution.stdout is not None:
            exec_harness['stdout'] = execution.stdout

        # key: 'exit_code'
        if execution.exit_code is not None:
            exec_harness['exit_code'] = execution.exit_code

        # key: 'make_env'
        if execution.compilation:
            make_env = {}
            for entry in execution.compilation.make_env:
                make_env[entry.key] = entry.value
            if len(make_env) > 0:
                exec_harness['make_env'] = make_env

        # key: 'system'
        system = {'platform': execution.sys_platform,
                  'cpu': {
                      'cores_logical': execution.sys_cpu_logical,
                      'cores_physical': execution.sys_cpu_physical,
                  },
                  'memory': {
                      'available': execution.sys_mem_avail,
                      'free': execution.sys_mem_free,
                      'total': execution.sys_mem_total,
                      'used': execution.sys_mem_used
                  }}

        cpu_freq = []
        cpu_percent = []
        for idx, core in enumerate(execution.sys_cpu):
            assert idx == core.idx
            cpu_freq.append([core.cur_clock, core.min_clock, core.max_clock])
            cpu_percent.append(core.percent)

        system['cpu']['freq'] = cpu_freq
        system['cpu']['percent'] = cpu_percent

        exec_harness['system'] = system

        # store harness
        if harness_name not in harness_data:
            harness_data[harness_name] = {}
        if config_name not in harness_data[harness_name]:
            harness_data[harness_name][config_name] = []
        harness_data[harness_name][config_name].append(exec_harness)

        # store a benchmark run of a specific execution
        for run in execution.runs:
            bench_name = run.benchmark.name

            fixture, name = split_benchmark_name(bench_name)
            exec_benchmark = {'clock_resolution': run.clock_resolution,
                              'clock_resolution_measured': run.clock_resolution_measured,
                              'clock_type': run.clock_type,
                              'disabled': run.disabled,
                              'fixture': fixture,
                              'harness': harness_name,
                              'iterations_per_run': run.iterations_per_run,
                              'mean': None,  # TODO: remove
                              'name': name,
                              'std_dev': None  # TODO: remove
                              }

            runs = []
            for dp in run.datapoints:
                assert (dp.idx >= 0) and (len(runs)-1 <= dp.idx <= len(runs))
                if dp.idx >= len(runs):
                    runs.append({})
                runs[dp.idx][dp.key] = dp.value

            exec_benchmark['runs'] = runs

            # store benchmark
            if bench_name not in benchmark_data:
                benchmark_data[bench_name] = {}
            if config_name not in benchmark_data[bench_name]:
                benchmark_data[bench_name][config_name] = []
            benchmark_data[bench_name][config_name].append(exec_benchmark)

    file_data = {'benchmark_data': benchmark_data, 'harness_data': harness_data}

    json.dump(file_data, file, sort_keys=True, indent=4, separators=(',', ': '))
