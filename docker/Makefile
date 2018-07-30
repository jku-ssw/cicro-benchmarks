GIT_TAG = "master"
SULONG_GIT_TAG = "master"

.PHONY: build

# Docker containers for various clang versions
build_BENCHMARK_CLANG-4:
	$(MAKE) -C BENCHMARK_CLANG-4 build GIT_TAG=$(GIT_TAG)

build_BENCHMARK_CLANG-5:
	$(MAKE) -C BENCHMARK_CLANG-5 build GIT_TAG=$(GIT_TAG)

build_BENCHMARK_CLANG-6:
	$(MAKE) -C BENCHMARK_CLANG-6 build GIT_TAG=$(GIT_TAG)

# Docker containers for compcert
build_BENCHMARK_COMPCERT:
	$(MAKE) -C BENCHMARK_COMPCERT build GIT_TAG=$(GIT_TAG)

# Docker containers for various gcc versions
build_BENCHMARK_GCC-5:
	$(MAKE) -C BENCHMARK_GCC-5 build GIT_TAG=$(GIT_TAG)

build_BENCHMARK_GCC-6:
	$(MAKE) -C BENCHMARK_GCC-6 build GIT_TAG=$(GIT_TAG)

build_BENCHMARK_GCC-7:
	$(MAKE) -C BENCHMARK_GCC-7 build GIT_TAG=$(GIT_TAG)

build_BENCHMARK_GCC-8:
	$(MAKE) -C BENCHMARK_GCC-8 build GIT_TAG=$(GIT_TAG)

# Docker containers for sanitizers
build_BENCHMARK_SANITIZERS:
	$(MAKE) -C BENCHMARK_SANITIZERS build GIT_TAG=$(GIT_TAG)

# Docker containers for sulong
build_SULONG:
	$(MAKE) -C SULONG build SULONG_GIT_TAG=$(SULONG_GIT_TAG)

build_BENCHMARK_SULONG: build_SULONG
	$(MAKE) -C BENCHMARK_SULONG build GIT_TAG=$(GIT_TAG)

# Docker containers for tiny-C-compiler
build_BENCHMARK_TCC:
	$(MAKE) -C BENCHMARK_TCC build GIT_TAG=$(GIT_TAG)

# Docker containers for valgrind
build_BENCHMARK_VALGRIND:
	$(MAKE) -C BENCHMARK_VALGRIND build GIT_TAG=$(GIT_TAG)

# Helper targets
build_BENCHMARK_CLANG: build_BENCHMARK_CLANG-4 build_BENCHMARK_CLANG-5 build_BENCHMARK_CLANG-6
build_BENCHMARK_GCC: build_BENCHMARK_GCC-5 build_BENCHMARK_GCC-6 build_BENCHMARK_GCC-7 build_BENCHMARK_GCC-8

build: build_BENCHMARK_CLANG build_BENCHMARK_COMPCERT build_BENCHMARK_GCC build_BENCHMARK_SANITIZERS build_BENCHMARK_SULONG build_BENCHMARK_TCC build_BENCHMARK_VALGRIND
