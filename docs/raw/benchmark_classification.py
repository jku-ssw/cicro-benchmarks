#!/usr/bin/env python3

import argparse
import os
import re
import sqlite3
import sys


DEFAULT_DB_FILE = os.path.join(sys.path[0], 'database.db')


def print_classification_count(conn):
    c = conn.cursor()
    c.execute("""
    SELECT CLASSIFICATION, COUNT(*)
        FROM `GithubProjectUnfiltered`
        WHERE STATUS == "IMPLEMENTED"
        GROUP BY CLASSIFICATION
        ORDER BY COUNT(*) DESC
    """)

    # output table in latex format
    print(r"\begin{tabular}{ l c } \toprule")
    print(r"  type & projects \\ \midrule")
    for line in c.fetchall():
        print(r"  {btype} & {projects} \\".format(btype=line[0], projects=line[1]))
    print(r"\bottomrule \end{tabular}")


def print_classification_in_detail(conn, benchdir):
    c = conn.cursor()
    c.execute("""
    SELECT CLASSIFICATION, GITHUB_OWNER_NAME, GITHUB_PROJECT_NAME
        FROM `GithubProjectUnfiltered`
        WHERE STATUS == "IMPLEMENTED"
    """)

    benchmarks = {}
    for line in c.fetchall():
        btype = line[0]
        dir_name = "-".join(line[1:3]).replace('\\', '-').replace('/', '-') + "_test"

        if btype not in benchmarks:
            benchmarks[btype] = []

        with open(os.path.join(benchdir, dir_name + ".c"), 'r') as f:
            found_benchmarks = re.findall(r'BENCHMARK\s*\(\s*(\w+)\s*,\s*(\w+)\s*,\s*(\d+)\s*,\s*(\d+)\s*\)', f.read())

        benchmarks[btype].append({dir_name: len(found_benchmarks)})

    rows = []
    for btype, b in benchmarks.items():
        n_benchmarks = sum([n for v in b for n in v.values()])
        latex_type = btype.replace('_', '\_')
        rows.append((latex_type, len(b), n_benchmarks))
    rows.sort(key=lambda r: r[2], reverse=True)
    rows.sort(key=lambda r: r[1], reverse=True)

    print(r"\begin{tabular}{ l c c } \toprule")
    print(r"  type & projects & benchmarks \\ \midrule")
    for row in rows:
       print(r"  {} & {} & {}\\".format(*row))
    print(r"  \hline")
    print(r"  sum & {} & {}\\".format(sum([r[1] for r in rows]), sum([r[2] for r in rows])))
    print(r"\bottomrule\end{tabular}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Print some statistics about the database')

    parser.add_argument('benchdir', metavar='BENCHDIR', nargs='?', type=str,
                        help='directory which contains all the benchmarks')
    parser.add_argument('--database', metavar='FILE', type=str, nargs='?', default=DEFAULT_DB_FILE,
                        help='filepath to database')

    args = parser.parse_args()

    if args.benchdir is not None and not os.path.isdir(args.benchdir):
        parser.error('"{}" is not a directory'.format(args.benchdir))

    if not os.path.isfile(args.database):
        parser.error('"{}" is not a file'.format(args.database))

    conn = sqlite3.connect(args.database)

    if args.benchdir is None:
        print_classification_count(conn)
    else:
        print_classification_in_detail(conn, args.benchdir)
