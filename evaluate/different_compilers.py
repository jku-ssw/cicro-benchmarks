#!/usr/bin/env python3

import argparse
import os
import re
import sqlite3
import subprocess


DATABASE_FILE = "evaluation.db"

COMPILERS = {
    "gcc" : {"make": {"CC": "gcc", "AS": "as", "CFLAGS": "", "LDFLAGS": ""}},
    "clang" : {"make": {"CC": "clang", "AS": "clang", "CFLAGS": "", "LDFLAGS": ""}},
    #"valgrind" : {"make": {"CC": "clang", "AS": "clang", "CFLAGS": "", "LDFLAGS": ""}, "exec": "valgrind"}
}

class EvaluationDb(object):
    def __init__(self):
        self.conn = sqlite3.connect(DATABASE_FILE)
        self.conn.execute("PRAGMA journal_mode=WAL")

        self.create_tables()

        self.c = self.conn.cursor()

    def create_tables(self):
        query = """CREATE TABLE IF NOT EXISTS`EVALUATION_RAW` (
            `NAME`	TEXT NOT NULL,
            `TESTCASE`	INTEGER NOT NULL,
            `EXECUTION_TIME_MS`	REAL NOT NULL,
            PRIMARY KEY(`NAME`,`TESTCASE`)
        );"""
        self.conn.execute(query)

    def add_entry(self, name, testcase, execution_time):
        query = """INSERT INTO EVALUATION_RAW (
            NAME,
            TESTCASE,
            EXECUTION_TIME_MS
            )

            VALUES(?, ?, ?);

        """

        tupel = (
            name,
            testcase,
            execution_time
        )

        try:
            self.c.execute(query, tupel)
            self.conn.commit()
        except sqlite3.Error as e:
            print(e)


def run_benchmark(workdir, file, exec=None):
    print(" * Run Benchmark for: {}".format(file))
    process = subprocess.Popen(["./{}".format(file)], cwd=workdir, stdout=subprocess.PIPE)
    stdout, _ = process.communicate()

    exec_time = float(re.findall(r"execution time: ([0-9\.]+)ms", str(stdout))[0])

    print("   Finished: {}ms".format(exec_time))

    return exec_time


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Execute tests for all given compilers with their parameters')
    parser.add_argument('testdir', type=str, help='directory where the tests and the Makefile is contained', action='store')
    #parser.add_argument('testcases', type=str, help='list of testcases to execute', action='store')

    args = parser.parse_args()

    if not os.path.isdir(args.testdir):
        print('"{0}" is not an existing directory!'.format(os.path.realpath(args.testdir)))
        exit(1)

    db = EvaluationDb()

    for compiler in COMPILERS:
        params = COMPILERS[compiler].get('make', {})

        # clean directory
        process = subprocess.Popen(['make', 'clean'], cwd=args.testdir, stdout=subprocess.DEVNULL)
        process.communicate()

        make_params = []
        for key  in params:
            make_params.append("{}={}".format(key, params[key]))

        # build all tests
        process = subprocess.Popen(['make'] + make_params, cwd=args.testdir)
        process.communicate()

        # execute all tests
        for testcase in os.listdir(args.testdir):
            if not testcase.endswith("_test"):
                continue
            exec_time = run_benchmark(args.testdir, testcase, COMPILERS[compiler].get('exec', None))

            db.add_entry(compiler, testcase, exec_time)