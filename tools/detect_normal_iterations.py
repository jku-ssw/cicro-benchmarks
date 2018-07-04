#!/usr/bin/env python3

import json
import os
import re
import sys

from util.color_logger import get_logger


logger = get_logger('merge_results')


BASE_DIR = os.path.abspath(os.path.join(sys.path[0], '..'))
BENCHMARK_DIR = os.path.join(BASE_DIR, 'benchmarks')


def evaluate_benchmark(workdir, harness):
    filepath = os.path.join(workdir, harness)

    benchmarks = {}

    with open(filepath, 'r') as f:
        for line in f:
            m = re.match(r'BENCHMARK\s*\(\s*(\w+)\s*,\s*(\w+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\)', line)
            if m:
                name = '{}-{}'.format(m.group(1), m.group(2))
                iterations = int(m.group(3))

                benchmarks[name] = {'iterations': iterations}

    return benchmarks


if __name__ == "__main__":

    harnesses = {}

    for file in os.listdir(BENCHMARK_DIR):
        if file.endswith('_test.c'):
            benchmarks = evaluate_benchmark(BENCHMARK_DIR, file)

            if benchmarks:
                harnesses[file] = benchmarks

    print(json.dumps(harnesses))
