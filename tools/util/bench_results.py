import json

from .color_logger import get_logger

logger = get_logger('bench_results' )


class BenchmarkingResults(object):
    def __init__(self):
        """Some generic class which allows loading/storing/interaction with benchmarking results

        data is stored in the following format:

        "benchmark-harness": {
            "runtime" : {
                # json data outputted by harness
            }
        }
        """
        self.data = {}

    def load_file(self, file):
        new_data = json.load(file)

        for harness, harness_data in new_data.items():
            for runtime, runtime_data in harness_data.items():
                self.set_single_run(harness, runtime, runtime_data, overwrite=False, very_verbose=False)

    def store_file(self, file):
        json.dump(self.data, file)

    def set_single_run(self, harness_name, runtime, data, overwrite=False, very_verbose=True):
        if harness_name not in self.data:
            self.data[harness_name] = {}

        if runtime in self.data[harness_name]:
            if overwrite:
                logger.warning('benchmark run of "%s:%s" already present, overwrite data', harness_name, runtime)
            else:
                logger.error('benchmark run of "%s:%s" already present, skip writing', harness_name, runtime)
                logger.info('data: %s', data)
                return
        logger.debug('set benchmark run of "%s:%s"', harness_name, runtime)
        if very_verbose:
            logger.debug('data: "%s"', data)
        self.data[harness_name][runtime] = data

    def is_run_present(self, harness, runtime):
        return runtime in self.data.get(harness, {})

    def get_single_run(self, harness, runtime):
        return self.data[harness][runtime]

    def get_all_runs(self, runtime):
        for harness, harness_data in self.data.items():
            if runtime in harness_data:
                yield harness_data[runtime]

    def get_all_benchmarks_of_runtime(self, runtime):
        for run in self.get_all_runs(runtime):
            for benchmark in run.get('benchmarks', []):
                print(benchmark)

    @staticmethod
    def benchmark_name(benchmark):
        return "{}.{}".format(benchmark.get('fixture'), benchmark.get('name'))

    def get_all_benchmark_names(self):
        benchmarks = set()
        for harness_data in self.data.values():
            for run in harness_data.values():
                for benchmark in run.get('benchmarks', []):
                    benchmarks.add(BenchmarkingResults.benchmark_name(benchmark))

        return benchmarks

    def get_harness(self, harness):
        return self.data[harness]

    def get_all_runtimes(self):
        runtimes = set()

        for _, harness_data in self.data.items():
            runtimes.update(harness_data.keys())

        return runtimes

    def get_all_harness(self):
        return self.data.keys()