import functools
import re

import numpy

from util.color_logger import get_logger


logger = get_logger('plot')


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
            baseline_duration_arr = numpy.array([r['duration'] for r in baseline_full_arr])

            baseline_mean = numpy.mean(baseline_duration_arr)
            baseline_std_dev = numpy.std(baseline_duration_arr)

            assert baseline_mean != 0
            assert baseline_std_dev != 0

            for runtime, data in runs.items():
                if runtime == baseline:
                    continue

                if not re.match(filter_runtime, runtime):
                    continue

                benchmark_full_arr = functools.reduce(list.__add__, [run['runs'] for run in data])
                benchmark_duration_arr = numpy.array([r['duration'] for r in benchmark_full_arr])

                normalized_mean = numpy.mean(benchmark_duration_arr) / baseline_mean
                normalized_std_dev = numpy.std(benchmark_duration_arr) / baseline_std_dev

                assert normalized_mean != 0
                assert normalized_std_dev != 0

                processed_data[benchmark_name][runtime] = {'mean': normalized_mean,
                                                           'std_dev': normalized_std_dev}
        except:  # NOQA: E722
            logger.exception('exception occured while preprocessing benchmark "%s"', benchmark_name)

    return processed_data
