#!/usr/bin/env python3

import argparse

from urllib.parse import urlparse


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Build a simple Makefile entry.')
    parser.add_argument('url', type=str, help='url to github project', action='store')

    args = parser.parse_args()

    url = args.url
    name = urlparse(url).path[1:].replace('\\', '-').replace('/', '-')

    print()
    print("########## {url}".format(url=url))
    print()
    print("{name}_$(TEST_SUFFIX): {name} {name}_test.c $(HARNESS_C)".format(name=name))
    print("	$(CC) $(HARNESS_C) {name}_test.c -o \"$(TEST_DIR)/{name}_$(TEST_SUFFIX)\"".format(name=name))
    print()
