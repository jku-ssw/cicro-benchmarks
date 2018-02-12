#!/usr/bin/env python3

import argparse
import csv
import json
import os
import re
import subprocess
import sys


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Count lines of code for all given directories')
    parser.add_argument('testdir', type=str, help='directory where the tests and the Makefile is contained', action='store')

    args = parser.parse_args()

    if not os.path.isdir(args.testdir):
        print('"{0}" is not an existing directory!'.format(os.path.realpath(args.testdir)))
        exit(1)

    spamwriter = csv.writer(sys.stdout) # , delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL
    spamwriter.writerow(['project', 'ASM_LOC', 'C_LOC', 'CPP_LOC', 'H_LOC'])

    # execute all tests
    for testcase in sorted(os.listdir(args.testdir)):
        if not testcase.endswith("_test.c"):
            continue

        try:
            testdir = testcase[:-7]
        
            process = subprocess.Popen(['cloc', testdir, "--json"], cwd=args.testdir, stdout=subprocess.PIPE)
            stdout, _ = process.communicate()
            j = json.loads(stdout)
            
            ASM_LOC = j.get('Assembly', {}).get('code', 0)
            C_LOC = j.get('C', {}).get('code', 0)
            CPP_LOC = j.get('C++', {}).get('code', 0)
            H_LOC = j.get('C/C++ Header', {}).get('code', 0)
            
            spamwriter.writerow([testdir, ASM_LOC, C_LOC, CPP_LOC, H_LOC])
            
        except RuntimeError:
            print("   FAILED!")
