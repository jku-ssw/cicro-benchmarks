BENCHMARK_EXEC_ARGS =
BENCHMARK_EXEC_ARGS_PAPI = $(BENCHMARK_EXEC_ARGS) --papi=PAPI_LD_INS,PAPI_SR_INS,PAPI_BR_INS,PAPI_TOT_INS,PAPI_TOT_CYC

BENCHMARK_RUNS = 1

BENCHMARK_BASE_RUNTIME = clang-O3-v7  # TODO: change

OUTPUT_DIR_NAME = data
OUTPUT_DIR = $(CURDIR)/$(OUTPUT_DIR_NAME)

DOCKER_DIR = $(CURDIR)/../docker
TOOLS_DIR = $(CURDIR)/../tools

CLANG_TARGET_NAMES = clang-3.3 clang-3.4 clang-3.5 clang-3.6 clang-3.7 clang-3.8 clang-3.9 clang-4.0 clang-5.0 clang-6.0 clang-7.0
CLANG_TARGET_JSON = $(CLANG_TARGET_NAMES:=.json)
CLANG_TARGET_PATHS = $(addprefix $(OUTPUT_DIR_NAME)/, $(CLANG_TARGET_JSON))

GCC_TARGET_NAMES = gcc-4.8 gcc-5 gcc-6 gcc-7 gcc-8
GCC_TARGET_JSON = $(GCC_TARGET_NAMES:=.json)
GCC_TARGET_PATHS = $(addprefix $(OUTPUT_DIR_NAME)/, $(GCC_TARGET_JSON))


ALL_TARGET_NAMES = $(CLANG_TARGET_NAMES) $(GCC_TARGET_NAMES) drmemory compcert sanitizers lowfat tcc memcheck callgrind qemu softboundcets sulong boehmgc dmalloc diehard jemalloc hoard freeguard libdislocator tcmalloc dlmalloc tlsf
ALL_TARGET_JSON = $(ALL_TARGET_NAMES:=.json)
ALL_TARGET_PATHS = $(addprefix $(OUTPUT_DIR_NAME)/, $(ALL_TARGET_JSON))
HIGH_TIMEOUT = --timeout 7200
LLVM_VERSION_COMPARISON_RUNTIMES = clang-O3
GCC_VERSION_COMPARISON_RUNTIMES = gcc-O3

PLOTS = tools-overview.pdf  LLVM-versions.pdf GCC-versions.pdf compiler-flags.pdf Rplots.pdf instruction-mix-clang.pdf instruction-mix-gcc.pdf memory-allocators.pdf
PLOT_PATHS = $(addprefix $(OUTPUT_DIR_NAME)/, $(PLOTS))

.PHONY: all build clean

all: build $(ALL_TARGET_PATHS) $(OUTPUT_DIR_NAME)/benchmark-classification.tex $(PLOT_PATHS) $(OUTPUT_DIR_NAME)/loc.csv

build:
	$(MAKE) -C $(DOCKER_DIR) build

clean:
	rm -f $(OUTPUT_DIR_NAME)/*.json $(OUTPUT_DIR_NAME)/Rplots.pdf

$(OUTPUT_DIR_NAME)/benchmark-classification.tex:
	../docs/raw/benchmark_classification.py > $(OUTPUT_DIR)/benchmark-classification.tex

# benchmark clang
$(OUTPUT_DIR_NAME)/clang-3.3.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.3 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.4.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.4 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.5.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.5 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.6.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.7.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.7 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.8.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-3.9.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-3.9 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-4.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-4 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-5.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-5 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-6.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime '$(LLVM_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/clang-7.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-7 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --filter-runtime clang"

# benchmark compcert
$(OUTPUT_DIR_NAME)/compcert.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_COMPCERT run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --ignore-errors"

# benchmark dmalloc
$(OUTPUT_DIR_NAME)/dmalloc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_DMALLOC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --ignore-errors"

$(OUTPUT_DIR_NAME)/freeguard.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_FREEGUARD run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS)' --ignore-errors --filter-harness '(?!(wolkykim-qlibc))'" # wolkykim-qlibc gets stuck during execution

# benchmark gcc
$(OUTPUT_DIR_NAME)/gcc-4.8.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-4.8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-5.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-5 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-6.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-7.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-7 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --ignore-errors --filter-runtime '$(GCC_VERSION_COMPARISON_RUNTIMES)'"

$(OUTPUT_DIR_NAME)/gcc-8.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --ignore-errors $(HIGH_TIMEOUT)" # high timeout for MPX

# benchmark sanitizers (based on clang-6.0)
$(OUTPUT_DIR_NAME)/sanitizers.json: $(OUTPUT_DIR_NAME)/asan.json $(OUTPUT_DIR_NAME)/msan.json $(OUTPUT_DIR_NAME)/tsan.json $(OUTPUT_DIR_NAME)/ubsan.json
	rm -f $(OUTPUT_DIR_NAME)/sanitizers.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="asan.json msan.json tsan.json ubsan.json sanitizers.json"

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
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_TCC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_ARGS="--exec-args='$(BENCHMARK_EXEC_ARGS_PAPI)' --ignore-errors"

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


#no-papi.json:
#	../tools/bench.py -y --filter-runtime="(gcc|clang)-(O0|O3)" no-papi.json --no-papi --suffix='-no-papi' --runs=${RUNS}

$(OUTPUT_DIR_NAME)/merged.json: $(ALL_TARGET_PATHS)
	rm -f $(OUTPUT_DIR_NAME)/merged.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="$(ALL_TARGET_JSON) merged.json"

$(OUTPUT_DIR_NAME)/overview.json: $(OUTPUT_DIR_NAME)/merged.json
	rm -f $(OUTPUT_DIR_NAME)/overview.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="--filter-runtime='boehmgc-O3|clang-O3-v7|gcc-O3-v8|gcc-mpx-O3-v8|gcc-gcov-O0-v8|asan-O3|msan-O3|tsan-O3|ubsan-O3|compcert-O|lowfat-O3|softboundcets-O3|sulong-graalvm|tcc|valgrind-O3|callgrind-O3|drmemory-O3|qemu-O3|dmalloc-O3|diehard-O3|jemalloc-O3|hoard-O3|freeguard-O3|libdislocator-O3' merged.json overview.json"

$(OUTPUT_DIR_NAME)/memory-allocators.json: $(OUTPUT_DIR_NAME)/merged.json
	rm -f $(OUTPUT_DIR_NAME)/memory-allocators.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="--filter-runtime='clang-O3-v7|boehmgc-O3|dmalloc-O3|diehard-O3|jemalloc-O3|hoard-O3|freeguard-O3|libdislocator-O3|dlmalloc-O3|tlsf-O3' merged.json memory-allocators.json"

$(OUTPUT_DIR_NAME)/llvm-versions.json: $(CLANG_TARGET_PATHS)
	rm -f $(OUTPUT_DIR_NAME)/llvm-versions.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="--filter-runtime='$(LLVM_VERSION_COMPARISON_RUNTIMES)' $(CLANG_TARGET_JSON) llvm-versions.json"

$(OUTPUT_DIR_NAME)/gcc-versions.json: $(GCC_TARGET_PATHS)
	rm -f $(OUTPUT_DIR_NAME)/gcc-versions.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="--filter-runtime='$(GCC_VERSION_COMPARISON_RUNTIMES)' $(GCC_TARGET_JSON) gcc-versions.json"

$(OUTPUT_DIR_NAME)/compiler-flags.json: $(GCC_TARGET_PATHS) $(CLANG_TARGET_PATHS) $(OUTPUT_DIR_NAME)/compcert.json
	rm -f $(OUTPUT_DIR_NAME)/compiler-flags.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="--filter-runtime='(gcc-O0-v8|gcc-O1-v8|gcc-O2-v8|gcc-O3-v8|gcc-Os-v8|clang-O0-v7|clang-O1-v7|clang-O2-v7|clang-O3-v7|clang-Os-v7|clang-Ofast-v7|clang-march-native-O3-v7|gcc-march-native-O3-v8|gcc-Ofast|compcert-O|compcert-OO|compcert-Os)' $(GCC_TARGET_JSON) $(CLANG_TARGET_JSON) compcert.json compiler-flags.json"

$(OUTPUT_DIR_NAME)/special-clang-flags.json: $(CLANG_TARGET_PATHS)
	rm -f $(OUTPUT_DIR_NAME)/special-clang-flags.json
	$(MAKE) -C $(DOCKER_DIR)/MERGE run OUTPUT_DIR=$(OUTPUT_DIR) MERGE_ARGS="--filter-runtime='(clang-O3-v7|clang-fno-delete-null-pointer-checks-O3-v7|clang-fno-strict-aliasing-O3-v7|clang-fno-strict-overflow-O3-v7|clang-mno-retpoline-O3-v7)' $(CLANG_TARGET_JSON) special-clang-flags.json"



$(OUTPUT_DIR_NAME)/loc.csv: $(OUTPUT_DIR_NAME)/gcc-8.json
	rm -f $(OUTPUT_DIR_NAME)/loc.csv
	../tools/extract_gcov.py --filter-runtime gcc-gcov-line-numbers-v8 $(OUTPUT_DIR_NAME)/gcc-8.json > $(OUTPUT_DIR_NAME)/loc.csv

$(OUTPUT_DIR_NAME)/Rplots.pdf: $(OUTPUT_DIR_NAME)/merged.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) PLOT_ARGS="--benchfile=/data/merged.json --base-runtime=$(BENCHMARK_BASE_RUNTIME)"

$(OUTPUT_DIR_NAME)/LLVM-versions.pdf: $(OUTPUT_DIR_NAME)/llvm-versions.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_compiler_versions.R" PLOT_ARGS="--benchfile=/data/llvm-versions.json --base-runtime=clang-O3-v3.3 --outputfile=/data/LLVM-versions.pdf"

$(OUTPUT_DIR_NAME)/GCC-versions.pdf: $(OUTPUT_DIR_NAME)/gcc-versions.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_compiler_versions.R" PLOT_ARGS="--benchfile=/data/gcc-versions.json --base-runtime=gcc-O3-v4.8 --outputfile=/data/GCC-versions.pdf"

$(OUTPUT_DIR_NAME)/compiler-flags.pdf: $(OUTPUT_DIR_NAME)/compiler-flags.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_compiler_versions.R" PLOT_ARGS="--benchfile=/data/compiler-flags.json --base-runtime=clang-O0-v7 --outputfile=/data/compiler-flags.pdf"

$(OUTPUT_DIR_NAME)/clang-compiler-flags.pdf: $(OUTPUT_DIR_NAME)/special-clang-flags.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_compiler_versions.R" PLOT_ARGS="--benchfile=/data/special-clang-flags.json --base-runtime=clang-O3-v7 --outputfile=/data/special-clang-flags.pdf"

$(OUTPUT_DIR_NAME)/tools-overview.pdf: $(OUTPUT_DIR_NAME)/overview.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_compiler_versions.R" PLOT_ARGS="--benchfile=/data/overview.json --base-runtime=clang-O3-v7 --outputfile=/data/tools-overview.pdf"

$(OUTPUT_DIR_NAME)/memory-allocators.pdf: $(OUTPUT_DIR_NAME)/memory-allocators.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_compiler_versions.R" PLOT_ARGS="--benchfile=/data/memory-allocators.json --base-runtime=clang-O3-v7 --outputfile=/data/memory-allocators.pdf"


$(OUTPUT_DIR_NAME)/instruction-mix-clang.pdf: $(OUTPUT_DIR_NAME)/clang-7.0.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_instruction_mix.R" PLOT_ARGS="--benchfile=/data/clang-7.0.json --base-runtime=clang-O3-v7 --outputfile=/data/instruction-mix-clang.pdf"

$(OUTPUT_DIR_NAME)/instruction-mix-gcc.pdf: $(OUTPUT_DIR_NAME)/gcc-8.json
	$(MAKE) -C $(DOCKER_DIR)/PLOT run OUTPUT_DIR=$(OUTPUT_DIR) DOCKER_RUN_ARGS="--entrypoint=/benchmarks/tools/plot_instruction_mix.R" PLOT_ARGS="--benchfile=/data/gcc-8.json --base-runtime=gcc-O3-v8 --outputfile=/data/instruction-mix-gcc.pdf"
