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

# Example Harness 

```
#include 
#include 
#include 
#include 

_attribute_ ((noinline))
int benchmark(int problemSize);

int getProblemsize();

int main() {
	int result = 0;
	struct timeval tv_start = (struct timeval ) malloc(sizeof(struct timeval));
	struct timeval tv_end = (struct timeval ) malloc(sizeof(struct timeval));
	int start = 0;
	int end = 0;
	long time;
	
	int i;
	int totalCount = 50;

	int problemSize;
	
	for (i = 0; i < totalCount; i++) {
		problemSize = getProblemsize();
		gettimeofday(tv_start, NULL);
		result = benchmark(problemSize);
		gettimeofday(tv_end, NULL);
		time = (tv_end->tv_sec-tv_start->tv_sec)*1000L+(long)((tv_end->tv_usec-tv_start->tv_usec)*0.001);
		printf("execution time: %ld\n", time);
	}
	free(tv_start);
	free(tv_end);
	fflush(stdout);
	return 0;
}
```

2018.01.18
./fetch_github/fetch_github/fetch_github.py "language:c stars:250..300" --clone-repo-dir ./projects/
