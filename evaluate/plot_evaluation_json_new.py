#!/usr/bin/env python3

import json

import matplotlib.pyplot as plt
import matplotlib.cm as cm
import operator as o

import numpy as np


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


def barplot(ax, dpoints):
    '''
    Create a barchart for data across different categories with
    multiple conditions for each category.
    
    @param ax: The plotting axes from matplotlib.
    @param dpoints: The data set as an (n, 4) numpy array
    
    @see: http://emptypipes.org/2013/11/09/matplotlib-multicategory-barchart/
    '''

    # Aggregate the conditions and the categories according to their
    # mean values
    conditions = [(c, np.mean(dpoints[dpoints[:,0] == c][:,2].astype(float))) 
                  for c in np.unique(dpoints[:,0])]
    categories = [(c, np.mean(dpoints[dpoints[:,1] == c][:,2].astype(float))) 
                  for c in np.unique(dpoints[:,1])]

    # sort the conditions, categories and data so that the bars in
    # the plot will be ordered by category and condition
    conditions = [c[0] for c in sorted(conditions, key=o.itemgetter(1))]
    categories = [c[0] for c in sorted(categories, key=o.itemgetter(1))]

    dpoints = np.array(sorted(dpoints, key=lambda x: categories.index(x[1])))

    # the space between each set of bars
    space = 0.3
    n = len(conditions)
    width = (1 - space) / (len(conditions))

    # Create a set of bars at each position
    for i,cond in enumerate(sorted(conditions)):
        indeces = range(1, len(categories)+1)
        vals = dpoints[dpoints[:,0] == cond][:,2].astype(np.float)
        errors = dpoints[dpoints[:,0] == cond][:,3].astype(np.float)
        pos = [j - (1 - space) / 2. + i * width for j in indeces]
        ax.bar(pos, vals, yerr=errors, width=width, label=cond, 
               color=cm.Accent(float(i) / n))

    # Set the x-axis tick labels to be equal to the categories
    ax.set_xticks(indeces)
    ax.set_xticklabels(categories)
    plt.setp(plt.xticks()[1], rotation=45, ha='right')

    # Add the axis labels
    ax.set_ylabel("Relative Runtime")
    #ax.set_xlabel("Structure")

    # Add a legend
    handles, labels = ax.get_legend_handles_labels()
    ax.legend(handles[::-1], labels[::-1], loc='upper left')

    fig = plt.gcf()
    fig.subplots_adjust(bottom=0.20)


def histogram(ax, dpoints, compiler):
    # Aggregate the conditions and the categories according to their
    # mean values
    conditions = [(c, np.mean(dpoints[dpoints[:,0] == c][:,2].astype(float))) 
                  for c in np.unique(dpoints[:,0])]
    categories = [(c, np.mean(dpoints[dpoints[:,1] == c][:,2].astype(float))) 
                  for c in np.unique(dpoints[:,1])]

    # sort the conditions, categories and data so that the bars in
    # the plot will be ordered by category and condition
    conditions = [c[0] for c in sorted(conditions, key=o.itemgetter(1))]
    categories = [c[0] for c in sorted(categories, key=o.itemgetter(1))]

    dpoints = np.array(sorted(dpoints, key=lambda x: categories.index(x[1])))

    vals = dpoints[dpoints[:, 0] == compiler][:, 2].astype(np.float)
    vals = list(filter(None, vals))  # filter elements which are zero
    plt.hist(vals, bins=6, density=True, weights=vals)  # weights=np.zeros_like(vals) + 1. / vals.size

    ax.set_ylabel(compiler)


def boxplot(ax, dpoints):
    # Aggregate the conditions and the categories according to their
    # mean values
    conditions = [(c, np.mean(dpoints[dpoints[:,0] == c][:,2].astype(float))) 
                  for c in np.unique(dpoints[:,0])]
    categories = [(c, np.mean(dpoints[dpoints[:,1] == c][:,2].astype(float))) 
                  for c in np.unique(dpoints[:,1])]

    # sort the conditions, categories and data so that the bars in
    # the plot will be ordered by category and condition
    conditions = [c[0] for c in sorted(conditions, key=o.itemgetter(1))]
    categories = [c[0] for c in sorted(categories, key=o.itemgetter(1))]

    dpoints = np.array(sorted(dpoints, key=lambda x: categories.index(x[1])))

    val_arr = []
    val_labels = []

    # Create a set of bars at each position
    for i,cond in enumerate(sorted(conditions)):
        vals = dpoints[dpoints[:, 0] == cond][:, 2].astype(np.float)
        vals = list(filter(None, vals))  # filter elements which are zero
        val_arr.append(vals)
        val_labels.append(cond)

    ax.boxplot(val_arr, vert=False, notch=1, labels=val_labels)


def print_metrics(dpoints):
    compilers = {}

    for p in dpoints:
        compiler, name, mean, std_dev = p

        if compiler not in compilers:
            compilers[compiler] = {'mean': [], 'std_dev': []}

        if mean:
            compilers[compiler]['mean'].append(mean)
        if std_dev:
            compilers[compiler]['std_dev'].append(std_dev)

    print(" |----------------------||--------|---------|---------|")
    print(" | {:<20} || {:<6} | {:<7} | {:<7} |".format('benchmark', 'points', 'runtime', 'std_dev'))
    print(" |----------------------||--------|---------|---------|")
    for compiler in sorted(compilers):
        mean = compilers[compiler]['mean']
        std_dev = compilers[compiler]['std_dev']
        assert len(mean) == len(std_dev)
        n_points = len(mean)
        runtime = sum(mean) / len(mean)
        std_dev = sum(std_dev) / len(std_dev)
        print(" | {:<20} || {:<6} | {:<7.5f} | {:<7.5f} |".format(compiler, n_points, runtime, std_dev))
    print(" |----------------------||--------|---------|---------|")


if __name__ == "__main__":
    bench_results = parse_file(OUTPUT_FILE)

    # init list of compilers
    compilers = set()
    for _, value in sorted(bench_results.items()):
        compilers.update(value)

    dpoints_raw = []

    for key, value in sorted(bench_results.items()):
        if key in ['yosefk.checkedthreads', 'otree.alloc_free']:  # 'mbrossard.threadpool', 'lucasb.heatmap', 'codeplea.tinyexpr', 'rax.tree'
            print('ignore testcase: \"{}\"'.format(key))
            continue
        normalize_val = value.get('clang-1', {}).get('mean')
        if normalize_val is None:
            print('ignore  benchmark!!!')
            continue  # Ignore benchmark
        else:
            normalize_val = float(normalize_val)

        for compiler in compilers:
            mean = value.get(compiler, {}).get('mean')
            std_dev = value.get(compiler, {}).get('std_dev')

            if mean is None or std_dev is None:
                print('ignore compiler: \"{}\" in testcase: \"{}\"'.format(compiler, key))
                dpoints_raw.append([compiler, key, 0, 0])
                continue

            mean_normalized = float(mean) / normalize_val
            std_dev_normalized = float(std_dev) / normalize_val if std_dev else None

            dpoints_raw.append([compiler, key, mean_normalized, std_dev_normalized])

    print_metrics(dpoints_raw)

    dpoints = np.array(dpoints_raw)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    barplot(ax, dpoints)
    plt.show()

    #fig = plt.figure()
    #i = 0
    #ax = None
    #for compiler in sorted(compilers):
    #    i += 1
    #    ax = fig.add_subplot(len(compilers)*100 + 10 + i, sharex=ax)
    #    histogram(ax, dpoints, compiler)
    #plt.show()

    fig = plt.figure()
    ax = fig.add_subplot(111)
    boxplot(ax, dpoints)
    plt.show()

