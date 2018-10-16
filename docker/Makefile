GIT_TAG = "master"
SULONG_GIT_TAG = "master"

.PHONY: build

# Docker container which contains the benchmarking suite
build_BENCHMARK_BASE:
	docker build -t "cicro/benchmark-base" ../

# Docker containers for various clang versions
build_BENCHMARK_CLANG-3.3: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-3.3 build

build_BENCHMARK_CLANG-3.4: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-3.4 build

build_BENCHMARK_CLANG-3.5: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-3.5 build

build_BENCHMARK_CLANG-3.6: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-3.6 build

build_BENCHMARK_CLANG-3.7: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-3.7 build

build_BENCHMARK_CLANG-3.8: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-3.8 build

build_BENCHMARK_CLANG-3.9: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-3.9 build

build_BENCHMARK_CLANG-4: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-4 build

build_BENCHMARK_CLANG-5: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-5 build

build_BENCHMARK_CLANG-6: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-6 build

build_BENCHMARK_CLANG-7: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_CLANG-7 build

# Docker containers for compcert
build_BENCHMARK_COMPCERT: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_COMPCERT build

# Docker container for dr memory
build_BENCHMARK_DRMEMORY: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_DRMEMORY build

# Docker containers for various gcc versions
build_BENCHMARK_GCC-4.8: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_GCC-4.8 build

build_BENCHMARK_GCC-5: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_GCC-5 build

build_BENCHMARK_GCC-6: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_GCC-6 build

build_BENCHMARK_GCC-7: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_GCC-7 build

build_BENCHMARK_GCC-8: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_GCC-8 build

# Docker containers for lowfat
build_BENCHMARK_LOWFAT: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_LOWFAT build

# Docker container for QEMU
build_BENCHMARK_QEMU: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_QEMU build

# Docker containers for sanitizers
build_BENCHMARK_SANITIZERS: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_SANITIZERS build

# Docker containers for softbound
build_BENCHMARK_SOFTBOUNDCETS: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_SOFTBOUNDCETS build

# Docker container for the Boehm GC
build_BENCHMARK_BOEHMGC: build_BENCHMARK_BASE
	$(MAKE) -C build_BENCHMARK_BOEHMGC build

# Docker containers for sulong
build_SULONG:
	$(MAKE) -C SULONG build SULONG_GIT_TAG=$(SULONG_GIT_TAG)

build_BENCHMARK_SULONG: build_BENCHMARK_BASE build_SULONG
	$(MAKE) -C BENCHMARK_SULONG build

# Docker containers for tiny-C-compiler
build_BENCHMARK_TCC: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_TCC build

# Docker containers for valgrind
build_BENCHMARK_VALGRIND: build_BENCHMARK_BASE
	$(MAKE) -C BENCHMARK_VALGRIND build

# Docker container for various stuff of our harness
build_MERGE:
	$(MAKE) -C MERGE build GIT_TAG=$(GIT_TAG)

build_PLOT: build_BENCHMARK_BASE
	$(MAKE) -C PLOT build

# Helper targets
build_BENCHMARK_CLANG: build_BENCHMARK_CLANG-3.3 build_BENCHMARK_CLANG-3.4 build_BENCHMARK_CLANG-3.5 build_BENCHMARK_CLANG-3.6 build_BENCHMARK_CLANG-3.7 build_BENCHMARK_CLANG-3.8 build_BENCHMARK_CLANG-3.9 build_BENCHMARK_CLANG-4 build_BENCHMARK_CLANG-5 build_BENCHMARK_CLANG-6 build_BENCHMARK_CLANG-7
build_BENCHMARK_GCC: build_BENCHMARK_GCC-4.8 build_BENCHMARK_GCC-5 build_BENCHMARK_GCC-6 build_BENCHMARK_GCC-7 build_BENCHMARK_GCC-8

build_BENCHMARK: build_BENCHMARK_CLANG build_BENCHMARK_COMPCERT build_BENCHMARK_GCC build_BENCHMARK_SANITIZERS build_BENCHMARK_SULONG build_BENCHMARK_TCC build_BENCHMARK_LOWFAT build_BENCHMARK_BOEHMGC build_BENCHMARK_SOFTBOUNDCETS build_BENCHMARK_VALGRIND build_BENCHMARK_QEMU build_BENCHMARK_DRMEMORY

build: build_BENCHMARK build_MERGE build_PLOT
