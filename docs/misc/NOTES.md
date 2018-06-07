* write down the time needed for processing!

download all projects between X and Y stars -> insert into database
* no GPL projects
* no Projects with external dependency other than libc (or normally installed ones)
* only c projects
* runs on normal linux systems
* filter those that are larger than 10000 LOC -> view
* filter non-general purpose projects -> GENERAL_PURPOSE=0 in table
* filter those that use global variables not easy to reset -> SUCCESSFULLY_MODIFIED=0 in table

processing
* replace main function by a test runner invocation
* modify the test runner
* set up the variables
* adjust to run between 1 and 10 seconds with Clang -O0
* verify that it is not optimized away with Clang -O3
* verify that there are no memory leaks using Valgrind and MSan

build test/benchmark runner
* configure iterations -> should be possible both for benchmarking and to verify bug-finding tools

making available
* Github repo
* checks that projects can be build by wllvm
* check by Valgrind and MSan

# 2018.01.18
./fetch_github/fetch_github/fetch_github.py "language:c stars:250..300" --clone-repo-dir ./projects/

# 2018.01.26
./fetch_github/fetch_github/fetch_github.py "language:c stars:300..350" --clone-repo-dir ./projects/

# 2018.02.01
./fetch_github/fetch_github/fetch_github.py "language:c stars:350..400" --clone-repo-dir ./projects/

# 2018.02.13
./fetch_github/fetch_github/fetch_github.py "language:c stars:100..150" --clone-repo-dir ./projects/

# 2018-03.15

* Benchmark runner: export more metrics and all measurement points, similar to Hayai (https://bruun.co/2012/02/07/easy-cpp-benchmarking/)
* Decrase number of iterations per run, and overal runtime to allow more runs
* Add setup function
* calculate standard derivation to find out if a benchmark is really suitable

