#!/usr/bin/env python3

import sqlite3

import numpy as np
import matplotlib.pyplot as plt

DATABASE_FILE = "evaluation.db"

def get_datanase_content():
    conn = sqlite3.connect(DATABASE_FILE)
    conn.execute("PRAGMA journal_mode=WAL")

    c = conn.cursor()

    c.execute('SELECT NAME, TESTCASE, EXECUTION_TIME_MS FROM EVALUATION_RAW')

    bench_results = {}

    for res in c.fetchall():
        if res[1] not in bench_results:
            bench_results[res[1]] = {}

        bench_results[res[1]][res[0]] = res[2]

    return bench_results


if __name__ == "__main__":
    bench_results = get_datanase_content()

    y = {'clang': [], 'gcc': []}
    x = []

    for key, value in sorted(bench_results.items()):
        normalize_val = value.get('clang')
        if normalize_val is None:
            print('ignore  benchmark!!!')
            continue # Ignore benchmark

        x.append(key)
        for compiler in y:
            y[compiler].append(value.get(compiler, 0) / normalize_val)

    print(x)
    print(y)

    ind = np.arange(len(x))
    width = 0.35  # the width of the bars

    fig, ax = plt.subplots()
    i=0
    rects = {}
    for compiler in y:
        rects[compiler] = ax.bar(ind + i*width, y[compiler], width) # , color='r'
        i += 1

    ax.legend((0, 0), ('Men', 'Women'))

    ax.set_xticks(ind + width / 2)
    ax.set_xticklabels(x, rotation=60)

    ref_tupel = []
    name_tupel = []
    for compiler in rects:
        ref_tupel.append(rects[compiler])
        name_tupel.append(compiler)
    ax.legend(ref_tupel, name_tupel)

    plt.show()