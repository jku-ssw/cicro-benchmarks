#!/usr/bin/env python

import argparse
import json

import matplotlib.pyplot as plt


def plot_benchmark(bench):

    x = []
    y = []
    for run in bench.get('runs', []):
        x.append(len(x))
        y.append(run['duration'])

    y_mean = [bench.get('mean')]*len(x)

    fig, ax = plt.subplots()

    # plot measurement points
    markerline, stemlines, baseline = ax.stem(x, y, ' ')

    # Plot the average line
    meanline = ax.plot(x, y_mean, label='Mean', color='g', linestyle='--')

    plt.title('{}.{}'.format(bench.get('fixture'), bench.get('name')))

    plt.ylabel('time (us)')
    plt.xlabel('run')

    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Evaluate benchmark results which are outputted as json')
    parser.add_argument('benchfile', type=str, help='path to benchmark file', action='store')

    args = parser.parse_args()

    with open(args.benchfile, 'r') as f:
        benchmarks = json.loads(f.read())

    for bench in benchmarks.get('benchmarks', []):
        plot_benchmark(bench)
        #break  # TODO
