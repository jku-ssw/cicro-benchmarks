BENCHMARK_EXEC_ARGS =
BENCHMARK_EXEC_ARGS_PAPI = $(BENCHMARK_EXEC_ARGS) --papi=PAPI_LD_INS,PAPI_SR_INS,PAPI_BR_INS,PAPI_TOT_INS,PAPI_TOT_CYC

BENCHMARK_RUNS = 3

BENCHMARK_BASE_RUNTIME = clang-O3-v7  # TODO: change

OUTPUT_DIR_NAME = data
OUTPUT_DIR = $(CURDIR)/$(OUTPUT_DIR_NAME)

DOCKER_DIR = $(CURDIR)/../docker
TOOLS_DIR = $(CURDIR)/../tools

CLANG_TARGET_NAMES = clang-3.3 clang-3.4 clang-3.5 clang-3.6 clang-3.7 clang-3.8 clang-3.9 clang-4.0 clang-5.0 clang-6.0 clang-7.0 clang-8.0
CLANG_TARGET_JSON = $(CLANG_TARGET_NAMES:=.json)
CLANG_TARGET_PATHS = $(addprefix $(OUTPUT_DIR_NAME)/, $(CLANG_TARGET_JSON))

GCC_TARGET_NAMES = gcc-4.6 gcc-4.7 gcc-4.8 gcc-4.9 gcc-5 gcc-6 gcc-7 gcc-8
GCC_TARGET_JSON = $(GCC_TARGET_NAMES:=.json)
GCC_TARGET_PATHS = $(addprefix $(OUTPUT_DIR_NAME)/, $(GCC_TARGET_JSON))


ALL_TARGET_NAMES = $(CLANG_TARGET_NAMES) $(GCC_TARGET_NAMES) drmemory compcert lowfat tcc memcheck callgrind qemu softboundcets sulong boehmgc dmalloc diehard jemalloc hoard freeguard libdislocator tcmalloc dlmalloc tlsf strace rpmalloc polly clang-8.0-lld clang-8.0-gold introspection usr-bin-time
ALL_TARGET_JSON = $(ALL_TARGET_NAMES:=.json)
ALL_TARGET_PATHS = $(addprefix $(OUTPUT_DIR_NAME)/, $(ALL_TARGET_JSON))
HIGH_TIMEOUT = --timeout 7200
LLVM_VERSION_COMPARISON_RUNTIMES = clang-O3
GCC_VERSION_COMPARISON_RUNTIMES = gcc-O3

PLOTS = loc.pdf
PLOT_PATHS = $(addprefix $(OUTPUT_DIR_NAME)/, $(PLOTS))

.PHONY: all build clean

all: build $(ALL_TARGET_PATHS) $(OUTPUT_DIR_NAME)/branch-counters.json $(OUTPUT_DIR_NAME)/instruction-mix1.json $(OUTPUT_DIR_NAME)/instruction-mix2.json $(OUTPUT_DIR_NAME)/cache-misses.json $(OUTPUT_DIR_NAME)/benchmark-classification.tex $(PLOT_PATHS)

build:
	$(MAKE) -C $(DOCKER_DIR) build

clean:
	rm -f $(OUTPUT_DIR_NAME)/*.json $(OUTPUT_DIR_NAME)/Rplots.pdf

$(OUTPUT_DIR_NAME)/benchmark-classification.tex:
	../docs/raw/benchmark_classification.py > $(OUTPUT_DIR)/benchmark-classification.tex

# benchmark clang
$(OUTPUT_DIR_NAME)/clang-3.3.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.3 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.4.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.4 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.5.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.5 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.6.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.7.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.7 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.8.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.9.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.9 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-4.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-4 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-5.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-5 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-6.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-7.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-7 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-8.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime '(clang(?!(-lld|-gold)))'"

$(OUTPUT_DIR_NAME)/clang-8.0-lld.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-8-LLD run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime 'clang-lld' --ignore-errors"

$(OUTPUT_DIR_NAME)/clang-8.0-gold.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-8-GOLD-LD run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime 'clang-gold' --ignore-errors"


$(OUTPUT_DIR_NAME)/branch-counters.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/bench.py --privileged" BENCHMARK_ARGS="--exec-args='--papi=PAPI_BR_INS,PAPI_BR_UCN,PAPI_BR_CN,PAPI_BR_MSP,PAPI_BR_PRC' --filter-runtime clang-O3 /data/branch-counters.json"

$(OUTPUT_DIR_NAME)/instruction-mix1.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/bench.py --privileged" BENCHMARK_ARGS="--exec-args='--papi=PAPI_SR_INS,PAPI_TOT_INS' --filter-runtime clang-O3 /data/instruction-mix1.json"

$(OUTPUT_DIR_NAME)/instruction-mix2.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/bench.py --privileged" BENCHMARK_ARGS="--exec-args='--papi=PAPI_BR_INS,PAPI_LD_INS' --filter-runtime clang-O3 /data/instruction-mix2.json"


$(OUTPUT_DIR_NAME)/cache-misses.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/bench.py --privileged" BENCHMARK_ARGS="--exec-args='--papi=PAPI_L1_TCM,PAPI_L2_TCM,PAPI_L3_TCM' --filter-runtime clang-O3 /data/cache-misses.json"


# benchmark compcert
$(OUTPUT_DIR_NAME)/compcert.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_COMPCERT run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --ignore-errors"

# benchmark dmalloc
$(OUTPUT_DIR_NAME)/dmalloc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_DMALLOC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --ignore-errors"

$(OUTPUT_DIR_NAME)/freeguard.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_FREEGUARD run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --ignore-errors --filter-harness '(?!(wolkykim-qlibc))'" # wolkykim-qlibc gets stuck during execution

$(OUTPUT_DIR_NAME)/rpmalloc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_RPMALLOC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --ignore-errors"

$(OUTPUT_DIR_NAME)/polly.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_POLLY run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --ignore-errors"


$(OUTPUT_DIR_NAME)/usr-bin-time.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_USR-BIN-TIME run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors"

# benchmark gcc
$(OUTPUT_DIR_NAME)/gcc-4.6.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-4.6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-4.7.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-4.7 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-4.8.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-4.8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-4.9.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-4.9 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-5.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-5 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-6.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-7.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-7 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-8.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors $(HIGH_TIMEOUT) '/data/gcc-8.json'" # high timeout for MPX

$(OUTPUT_DIR_NAME)/gcov.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime gcc-gcov-line-numbers --ignore-errors '/data/gcov.json'"

$(OUTPUT_DIR_NAME)/introspection.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_INTROSPECTION run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--filter-runtime introspection-mpx-O3 --filter-harness '(?!(codeplea-tinyexpr|vmg-crustache|adamdunkels-ubasic))'  --ignore-errors $(HIGH_TIMEOUT) '/data/introspection.json'" # high timeout for MPX

$(OUTPUT_DIR_NAME)/asan.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_SANITIZERS run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime asan-O3 $(HIGH_TIMEOUT)" OUTPUT_FILE="/$(OUTPUT_DIR_NAME)/asan.json"

$(OUTPUT_DIR_NAME)/msan.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_SANITIZERS run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime msan-O3 $(HIGH_TIMEOUT)" OUTPUT_FILE="/$(OUTPUT_DIR_NAME)/msan.json"

$(OUTPUT_DIR_NAME)/tsan.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_SANITIZERS run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime tsan-O3 $(HIGH_TIMEOUT)" OUTPUT_FILE="/$(OUTPUT_DIR_NAME)/tsan.json"

$(OUTPUT_DIR_NAME)/ubsan.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_SANITIZERS run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime ubsan-O3 $(HIGH_TIMEOUT)" OUTPUT_FILE="/$(OUTPUT_DIR_NAME)/ubsan.json"

# benchmark sulong (based on clang-6.0)
$(OUTPUT_DIR_NAME)/sulong.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_SULONG run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime sulong --ignore-errors $(HIGH_TIMEOUT)"
	# TODO: --filter-runtime="(sulong-warmup-0|sulong-warmup-2)" --filter-harness="(?!(faragon-libsrt|armon-libart))" --runs=1  and iterations file

# benchmark tcc
$(OUTPUT_DIR_NAME)/tcc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_TCC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--ignore-errors"

# benchmark valgrind (based on clang-6.0)
$(OUTPUT_DIR_NAME)/memcheck.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_VALGRIND run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime valgrind-O3 --ignore-errors $(HIGH_TIMEOUT)" OUTPUT_FILE="/$(OUTPUT_DIR_NAME)/memcheck.json"

$(OUTPUT_DIR_NAME)/callgrind.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_VALGRIND run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime callgrind-O3 --ignore-errors $(HIGH_TIMEOUT)" OUTPUT_FILE="/$(OUTPUT_DIR_NAME)/callgrind.json"

$(OUTPUT_DIR_NAME)/qemu.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_QEMU run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' $(HIGH_TIMEOUT)"

$(OUTPUT_DIR_NAME)/drmemory.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_DRMEMORY run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime drmemory-O3 --ignore-errors $(HIGH_TIMEOUT)"

$(OUTPUT_DIR_NAME)/lowfat.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_LOWFAT run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime lowfat-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/softboundcets.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_SOFTBOUNDCETS run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime softboundcets-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/boehmgc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_BOEHMGC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime boehmgc-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/diehard.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_DIEHARD run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime diehard-O3 --ignore-errors $(HIGH_TIMEOUT)"

$(OUTPUT_DIR_NAME)/jemalloc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_JEMALLOC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime jemalloc-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/hoard.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_HOARD run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime hoard-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/libdislocator.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_LIBDISLOCATOR run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime libdislocator-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/tcmalloc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_TCMALLOC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime tcmalloc-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/dlmalloc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_DLMALLOC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime dlmalloc-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/tlsf.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_TLSF run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime tlsf-O3 --ignore-errors"

$(OUTPUT_DIR_NAME)/strace.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_STRACE run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --filter-runtime strace-O3 --ignore-errors $(HIGH_TIMEOUT)"

$(OUTPUT_DIR_NAME)/merged.json: $(ALL_TARGET_PATHS)
	rm -f $(OUTPUT_DIR_NAME)/merged.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="$(ALL_TARGET_JSON) merged.json"

$(OUTPUT_DIR_NAME)/loc.csv: $(OUTPUT_DIR_NAME)/gcov.json
	rm -f $(OUTPUT_DIR_NAME)/loc.csv
	../tools/extract_gcov.py --filter-runtime gcc-gcov-line-numbers-v8 $(OUTPUT_DIR_NAME)/gcov.json > $(OUTPUT_DIR_NAME)/loc.csv

$(OUTPUT_DIR_NAME)/loc.pdf: $(OUTPUT_DIR_NAME)/loc.csv
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_loc.R" PLOT_ARGS="--inputfile=/data/loc.csv --outputfile=/data/loc.pdf"
