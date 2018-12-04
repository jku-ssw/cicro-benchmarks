import json
import re

from sqlalchemy.orm import selectinload

from dateutil.parser import parse as date_parse

import util.datamodel as dm
from util.color_logger import get_logger
from util.auto_extend_list import auto_extend_list

from datetime import timezone

logger = get_logger('datamodel_helper')


def get_benchmark_name(benchmark):
    fixture = benchmark.get('fixture')
    name = benchmark.get('name')
    assert ('.' not in fixture) and ('.' not in name)
    return "{}.{}".format(fixture, name)


def split_benchmark_name(benchmark_name):
    splits = benchmark_name.split('.')
    assert len(splits) == 2
    return splits  # fixure, name


def load_file_in_db(session, file):
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

            run_id = 0
            for entry in data:
                entry_h_data = harness_data.get(entry['harness'], {}).get(runtime, []) if 'harness' in entry else []
                h_data = entry_h_data[run_id] if len(entry_h_data) > run_id else {}

                harness_name = entry['harness']
                harness = session.query(dm.Harness).filter_by(name=harness_name).one_or_none()
                if harness is None:
                    harness = dm.Harness(name=harness_name)
                    session.add(harness)

                benchmark = session.query(dm.Benchmark).filter_by(name=get_benchmark_name(entry)).one_or_none()
                if benchmark is None:
                    benchmark = dm.Benchmark(name=get_benchmark_name(entry), harness=harness)
                    session.add(benchmark)
                assert benchmark.harness == harness

                config = session.query(dm.Configuration).filter_by(name=runtime, harness=harness).one_or_none()
                if config is None:
                    config = dm.Configuration(name=runtime, harness=harness)
                    session.add(config)

                h_system = h_data.get('system', {})
                h_cpu = h_system.get('cpu', {})
                h_memory = h_system.get('memory', {})

                execution_cache_arr = execution_cache.get(harness_name, {}).get(runtime, [])
                if run_id < len(execution_cache_arr) and execution_cache_arr[run_id] is not None:
                    execution = execution_cache[harness_name][runtime][run_id]
                else:
                    # store datetime in UTC
                    dt = date_parse(h_data['datetime']).astimezone(timezone.utc) if 'datetime' in h_data else None
                    execution = dm.Execution(configuration=config,
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
                    session.add(execution)

                    session.add_all([dm.ExecutionBuildSystem(execution=execution, key=key, value=value)
                                     for key, value in h_data.get('build_system', {}).items()])

                    session.add_all([dm.ExecutionMakeEnv(execution=execution, key=key, value=value)
                                     for key, value in h_data.get('make_env', {}).items()])

                    session.add_all([dm.ExecutionSystemCpu(execution=execution, idx=idx, percent=val[0],
                                                           cur_clock=val[1][0], min_clock=val[1][1],
                                                           max_clock=val[1][2])
                                     for idx, val in enumerate(zip(h_cpu.get('percent', []), h_cpu.get('freq', [])))])

                    # store in execution cache
                    if harness_name not in execution_cache:
                        execution_cache[harness_name] = {}
                    if runtime not in execution_cache[harness_name]:
                        execution_cache[harness_name][runtime] = auto_extend_list(None)
                    execution_cache[harness_name][runtime][run_id] = execution

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
    return True


def save_file_as_json(session, file, runtime_filter='.*'):
    benchmark_data = {}
    harness_data = {}

    # store all executions
    for execution in session.query(dm.Execution).options(selectinload(dm.Execution.configuration)).all():
        config_name = execution.configuration.name
        harness_name = execution.configuration.harness.name

        if not re.match(runtime_filter, config_name):
            logger.debug('"{}" does not match filter -> skip'.format(config_name))
            continue

        exec_harness = {}

        # key: 'build_system'
        build_system = {}
        for entry in execution.build_system:
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
        make_env = {}
        for entry in execution.make_env:
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
            exec_benchmark = {'clock_resulution': run.clock_resolution,
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
