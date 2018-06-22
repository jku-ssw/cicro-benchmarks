import functools
import re

import numpy

from util.color_logger import get_logger


logger = get_logger('plot')


def calculate_stat_from_array(array):
    if len(array) == 0:
        return {}

    res_dict = {}
    for type in array[0]:
        tmp_arr = numpy.array([r[type] for r in array])  # TODO: support runs with different types

        tmp_mean = numpy.mean(tmp_arr)

        if tmp_mean == 0:
            logger.error('no correct baseline calculated for', type)
            continue

        res_dict[type] = {'mean': tmp_mean}

    return res_dict


def preprocess(results, baseline, filter_runtime='.*', filter_benchmark='.*'):
    processed_data = {}
    for benchmark_name in results.get_all_benchmark_names():
        if not re.match(filter_benchmark, benchmark_name):
            continue

        try:
            runs = results.get_all_benchmark_runs(benchmark_name)
            if baseline not in runs:
                logger.warning('no baseline run found for "%s"', benchmark_name)
                continue

            processed_data[benchmark_name] = {}

            baseline_full_arr = functools.reduce(list.__add__, [run['runs'] for run in runs[baseline]])

            baseline_dict = calculate_stat_from_array(baseline_full_arr)

            for runtime, data in runs.items():
                if runtime == baseline:
                    continue

                if not re.match(filter_runtime, runtime):
                    continue

                benchmark_full_arr = functools.reduce(list.__add__, [run['runs'] for run in data])

                benchmark_dict = calculate_stat_from_array(benchmark_full_arr)

                processed_data[benchmark_name][runtime] = {}
                for type in benchmark_dict:
                    if type not in baseline_dict:
                        logger.warning('no baseline found for ', type)
                        continue

                    normalized_mean = benchmark_dict[type]['mean'] / baseline_dict[type]['mean']

                    assert normalized_mean != 0

                    processed_data[benchmark_name][runtime][type] = {'mean': normalized_mean}

        except:  # NOQA: E722
            logger.exception('exception occured while preprocessing benchmark "%s"', benchmark_name)

    return processed_data
