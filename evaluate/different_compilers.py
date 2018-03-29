#!/usr/bin/env python3

import argparse
import os
import re
import signal
import sqlite3
import subprocess
import sys
import time
import traceback
import json

OUTPUT_FILE = 'compiler_evaluation.json'


def parse_exec_output(stdout):
    return json.loads(stdout)
    #return float(re.findall(r"execution time: ([0-9\.]+)ms", str(stdout))[0])


def timeout_handling(process):
    print("    TIMEOUT")
    process.send_signal(signal.SIGKILL)
    process.terminate()
    process.wait()
    return None

def run_benchmark(workdir, file):
    time.sleep(1)
    process = subprocess.Popen(["./{}".format(file), '--output=json'], cwd=workdir, stdout=subprocess.PIPE)
    stdout, _ = process.communicate(timeout=60)

    if process.returncode != 0:
        return None

    return parse_exec_output(stdout)


def run_lli_benchmark(workdir, file):
    process_bc = subprocess.Popen(["extract-bc", file], cwd=workdir)
    process_bc.wait(timeout=10)

    process = subprocess.Popen(["lli",  "{}.bc".format(file), '--output=json'], cwd=workdir, stdout=subprocess.PIPE)
    stdout, _ = process.communicate(timeout=60)

    if process.returncode != 0:
        return None

    return parse_exec_output(stdout)


SULONG_EXEC_DIR = '/home/thomas/JKU/java-llvm-ir-builder-dev/sulong'
SULONG_TIMEOUT = 20*10
SULONG_1000_TIMEOUT = (100+10)*10

def run_sulong_benchmark(workdir, file):
    process_bc = subprocess.Popen(["extract-bc", file], cwd=workdir)
    process_bc.wait(timeout=10)

    time.sleep(1)
    process = subprocess.Popen(["mx", "-p", SULONG_EXEC_DIR, "--timeout={}".format(SULONG_TIMEOUT), "lli",  "{}.bc".format(file), '--output=json'], cwd=workdir, stdout=subprocess.PIPE)
    try:
        stdout, _ = process.communicate(timeout=SULONG_TIMEOUT+5)
    except subprocess.TimeoutExpired:
        return timeout_handling(process)

    if process.returncode != 0:
        return None

    return parse_exec_output(stdout)


def run_sulong_jdk_1000_benchmark(workdir, file):
    normal_exec_time = run_sulong_jdk_benchmark(workdir, file)

    if not normal_exec_time:
        return None

    print("    start full evalulation")

    #process_bc = subprocess.Popen(["extract-bc", file], cwd=workdir)
    #process_bc.wait(timeout=10)

    time.sleep(1)
    process = subprocess.Popen(["mx", "-p", SULONG_EXEC_DIR, "--timeout={}".format(SULONG_1000_TIMEOUT), "--jdk", "jvmci", "--dynamicimports=/compiler",
                                "lli",  "{}.bc".format(file), '--output=json', "--warmup=101",
                                "-Dgraal.TruffleCompilationThreshold=10"], cwd=workdir, stdout=subprocess.PIPE)
    try:
        stdout, _ = process.communicate(timeout=SULONG_1000_TIMEOUT+5)
    except subprocess.TimeoutExpired:
        print("    TIMEOUT")
        process.terminate()
        return None

    if process.returncode != 0:
        return None

    print("    full evalulation finished")

    return parse_exec_output(stdout)


def run_sulong_jdk_benchmark(workdir, file):
    process_bc = subprocess.Popen(["extract-bc", file], cwd=workdir)
    process_bc.wait(timeout=10)

    time.sleep(1)
    process = subprocess.Popen(["mx", "-p", SULONG_EXEC_DIR, "--timeout={}".format(SULONG_TIMEOUT), "--jdk", "jvmci", "--dynamicimports=/compiler",
                                "lli",  "{}.bc".format(file), '--output=json'], cwd=workdir, stdout=subprocess.PIPE)
    try:
        stdout, _ = process.communicate(timeout=SULONG_TIMEOUT+5)
    except subprocess.TimeoutExpired:
        print("    TIMEOUT")
        process.terminate()
        return None

    if process.returncode != 0:
        return None

    return parse_exec_output(stdout)


COMPILERS = {
    #"gcc" : {"make": {"CC": "gcc", "AS": "as", "CFLAGS": "", "LDFLAGS": ""}},
    #"clang" : {"make": {"CC": "clang", "AS": "clang", "CFLAGS": "", "LDFLAGS": ""}},
    #"gcc-O3" : {"make": {"CC": "gcc", "AS": "as", "CFLAGS": "-O3", "LDFLAGS": ""}},
    #"clang-O3" : {"make": {"CC": "clang", "AS": "clang", "CFLAGS": "-O3", "LDFLAGS": ""}},
    #"lli" : {"make": {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "", "LDFLAGS": ""}, "exec": run_lli_benchmark},
    "lli-O3" : {"make": {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "-O3", "LDFLAGS": ""}, "exec": run_lli_benchmark},
    #"sulong" : {"make": {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "", "LDFLAGS": ""}, "exec": run_sulong_benchmark},
    #"sulong-jdk" : {"make": {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "", "LDFLAGS": ""}, "exec": run_sulong_jdk_benchmark},
    #"sulong-jdk-O3" : {"make": {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "-O3", "LDFLAGS": ""}, "exec": run_sulong_jdk_benchmark},
    #"sulong-jdk-1000-O3" : {"make": {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "-O3", "LDFLAGS": ""}, "exec": run_sulong_jdk_1000_benchmark},
}


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Execute tests for all given compilers with their parameters')
    parser.add_argument('testdir', type=str, help='directory where the tests and the Makefile is contained', action='store')
    #parser.add_argument('testcases', type=str, help='list of testcases to execute', action='store')

    args = parser.parse_args()

    if not os.path.isdir(args.testdir):
        print('"{0}" is not an existing directory!'.format(os.path.realpath(args.testdir)))
        exit(1)

    data = {}

    if os.path.isfile(OUTPUT_FILE):
        print('"{0}" already exists!'.format(OUTPUT_FILE))
        with open(OUTPUT_FILE) as f:
            data = json.load(f)

    try:
        for compiler in COMPILERS:
            params = COMPILERS[compiler].get('make', {})

            # clean directory
            #process = subprocess.Popen(['make', 'clean'], cwd=args.testdir, stdout=subprocess.DEVNULL)
            #process.communicate()

            make_params = []
            for key  in params:
                make_params.append("{}={}".format(key, params[key]))

            # build all tests
            process = subprocess.Popen(['make'] + make_params, cwd=args.testdir)
            process.communicate()

            # execute all tests
            for testcase in sorted(os.listdir(args.testdir)):
                if not testcase.endswith("_test"):
                    continue

                print(" * Run Benchmark for: {}".format(testcase))
                try:
                    bench_func = COMPILERS[compiler].get('exec', run_benchmark)
                    exec_output = bench_func(args.testdir, testcase)

                    if testcase not in data:
                        data[testcase] = {}

                    data[testcase][compiler] = exec_output

                    print(exec_output)
                except RuntimeError:
                    print("   FAILED!")
                except (KeyboardInterrupt, SystemExit):
                    raise
                except:
                    traceback.print_exc(file=sys.stdout)
    finally:
        with open(OUTPUT_FILE, 'w') as f:
            f.write(json.dumps(data, indent=2))
