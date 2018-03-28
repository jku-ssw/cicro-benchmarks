#!/usr/bin/env python3

import json

import numpy as np
import matplotlib.pyplot as plt

OUTPUT_FILE = 'compiler_evaluation.json'

def parse_file(path):
    with open(path) as f:
        data = json.load(f)

    ret = {}

    for _, value in data.items():
        for compiler, benchmarks in value.items():
            if benchmarks is None:
                continue
            for benchmark in benchmarks.get('benchmarks', []):
                name = "{}.{}".format(benchmark['fixture'], benchmark['name'])
                
                if name not in ret:
                    ret[name] = {}
                
                ret[name][compiler] = benchmark

    return ret


if __name__ == "__main__":
    bench_results = parse_file(OUTPUT_FILE)

    y = {}
    x = []

    # init list of compilers
    for key, value in sorted(bench_results.items()):
        for compiler in sorted(value):
            if compiler not in y:
                y[compiler] = []

    for key, value in sorted(bench_results.items()):
        normalize_val = value.get('clang', {}).get('mean')
        if normalize_val is None:
            print('ignore  benchmark!!!')
            continue  # Ignore benchmark

        x.append(key.replace('_test', ''))
        for compiler in y:
            y[compiler].append(value.get(compiler, {}).get('mean', 0) / normalize_val)

    ind = np.arange(len(x))
    width = 0.90/len(y)  # the width of the bars

    fig, ax = plt.subplots()
    i=0
    rects = {}
    for compiler in y:
        rects[compiler] = ax.bar(ind + i*width, y[compiler], width) # , color='r'
        i += 1

    ax.set_xticks(ind + width * (len(y)-1) / 2)
    ax.set_xticklabels(x, rotation=45, ha='right')

    ref_tupel = []
    name_tupel = []
    for compiler in rects:
        ref_tupel.append(rects[compiler])
        name_tupel.append(compiler)
    ax.legend(ref_tupel, name_tupel)

    fig = plt.gcf()
    fig.subplots_adjust(bottom=0.15)

    plt.show()
