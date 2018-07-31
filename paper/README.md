# Paper

## Overview

This Makefile is able to do an reproducable benchmark run over all specified runtimes. Please note this can take quite a
while to finish.

```bash
sudo ../tools/ondemand.sh start  # switch into performance mode
sudo make all SULONG_GIT_TAG="master" GIT_TAG="master"  # build, run, merge, plot
```

All resuls are stored by default in the directory ```./data/```. Every docker container outputs its own file, which is
then combined into ```./data/merged.json```. In the end there is a big pdf with all plots at ```./data/Rplots.pdf```.

## Notes

To generate a LaTeX table, containing some basic statistics about the tests, execute the following command:

```bash
../docs/raw/some_stats.py ../benchmarks
```
