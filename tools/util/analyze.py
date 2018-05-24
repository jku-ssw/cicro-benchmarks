import logging

from util.color_logger import get_logger


logger = get_logger('plot' )


def preprocess(results, baseline):
    processed_data = {}
    for benchmark_name in results.get_all_benchmark_names():
        try:
            runs = results.get_all_benchmark_runs(benchmark_name)
            if baseline not in runs:
                logger.warning('no baseline run found for "%s"', benchmark_name)
                continue

            processed_data[benchmark_name] = {}

            baseline_mean = runs[baseline]['mean']
            baseline_std_dev = runs[baseline]['std_dev']

            assert baseline_mean != 0
            assert baseline_std_dev != 0

            for runtime, data in runs.items():
                if runtime == baseline:
                    continue

                normalized_mean = data['mean'] / baseline_mean
                normalized_std_dev = data['std_dev'] / baseline_std_dev

                assert normalized_mean != 0
                assert normalized_std_dev != 0

                processed_data[benchmark_name][runtime] = {'mean': normalized_mean,
                                                           'std_dev': normalized_std_dev}
        except:
            logger.exception('exception occured while preprocessing benchmark "%s"', benchmark_name)

    return processed_data