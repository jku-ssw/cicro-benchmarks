import re

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

            # TODO: add support for multiple runs
            baseline_mean = runs[baseline][0]['mean']
            baseline_std_dev = runs[baseline][0]['std_dev']

            assert baseline_mean != 0
            assert baseline_std_dev != 0

            for runtime, data in runs.items():
                if runtime == baseline:
                    continue

                if not re.match(filter_runtime, runtime):
                    continue

                normalized_mean = data[0]['mean'] / baseline_mean
                normalized_std_dev = data[0]['std_dev'] / baseline_std_dev

                assert normalized_mean != 0
                assert normalized_std_dev != 0

                processed_data[benchmark_name][runtime] = {'mean': normalized_mean,
                                                           'std_dev': normalized_std_dev}
        except:  # NOQA: E722
            logger.exception('exception occured while preprocessing benchmark "%s"', benchmark_name)

    return processed_data
