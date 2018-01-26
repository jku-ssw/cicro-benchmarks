#!/usr/bin/env python3

import argparse
import os
import sys

from urllib.parse import urlparse

BASE_TEST_PATH = os.path.join(sys.path[0], "make_test")

BASE_TEST_FILE = """#include "harness.h"

int __attribute__ ((noinline)) test_harness(void) {{
    return 0;
}}

int main(int argc, char* argv[]) {{
    _test_harness harness = {{
        .name="{name}",
        .description="", // TODO: missing
        .test_harness=*test_harness,
        .expected_runtime=0L // TODO: missing (ms)
    }};
    return _execute_harness(argc, argv, harness);
}}"""

def create_makefile_text(url, name):
    print()
    print("########## {url}".format(url=url))
    print()
    print("{name}_$(TEST_SUFFIX): {name} {name}_test.c $(HARNESS_C)".format(name=name))
    print("	$(CC) $(CFLAGS) $(HARNESS_C) {name}_test.c -o \"$(TEST_DIR)/{name}_$(TEST_SUFFIX)\" $(LDFLAGS)".format(name=name))
    print()


def create_initial_file(name):
    file_path = os.path.join(BASE_TEST_PATH, "{name}_test.c".format(name=name))
    if not os.path.isfile(file_path):
        with open(file_path, "w") as f:
            f.write(BASE_TEST_FILE.format(name=name))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Build a simple Makefile entry.')
    parser.add_argument('url', type=str, help='url to github project', action='store')

    args = parser.parse_args()

    url = args.url
    name = urlparse(url).path[1:].replace('\\', '-').replace('/', '-')

    create_makefile_text(url, name)
    create_initial_file(name)
