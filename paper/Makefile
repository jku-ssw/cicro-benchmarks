BENCHMARK_EXEC_ARGS =
BENCHMARK_EXEC_ARGS_PAPI = $(BENCHMARK_EXEC_ARGS) --papi=PAPI_LD_INS,PAPI_SR_INS,PAPI_BR_INS,PAPI_TOT_INS,PAPI_TOT_CYC

BENCHMARK_RUNS = 2

OUTPUT_DIR_NAME = data
OUTPUT_DIR = $(CURDIR)/$(OUTPUT_DIR_NAME)

DOCKER_DIR = $(CURDIR)/../docker

.PHONY: all build clean

all: build  # TODO: add all benchmark targets and then merge as well as plot

build:
	$(MAKE) -C $(DOCKER_DIR) build

clean:
	rm -f $(OUTPUT_DIR_NAME)/*.json Rplots.pdf

# benchmark clang
$(OUTPUT_DIR_NAME)/clang-4.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-4 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS_PAPI)"

$(OUTPUT_DIR_NAME)/clang-5.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-5 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS_PAPI)"

$(OUTPUT_DIR_NAME)/clang-6.0.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_CLANG-6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS_PAPI)"

# benchmark compcert
$(OUTPUT_DIR_NAME)/compcert.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_COMPCERT run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS)"

# benchmark gcc
$(OUTPUT_DIR_NAME)/gcc-5.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-5 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS_PAPI)"

$(OUTPUT_DIR_NAME)/gcc-6.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-6 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS_PAPI)"

$(OUTPUT_DIR_NAME)/gcc-7.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-7 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS_PAPI)"

$(OUTPUT_DIR_NAME)/gcc-8.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_GCC-8 run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS_PAPI)"

# benchmark sanitizers (based on clang-6.0)
$(OUTPUT_DIR_NAME)/sanitizers.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_SANITIZERS run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS)"

# benchmark sulong (based on clang-6.0)
$(OUTPUT_DIR_NAME)/sulong.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_SULONG run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS)"
	# TODO: --filter-runtime="(sulong-warmup-0|sulong-warmup-2)" --filter-harness="(?!(faragon-libsrt|armon-libart))" --runs=1  and iterations file

# benchmark tcc
$(OUTPUT_DIR_NAME)/tcc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_TCC run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS_PAPI)"

# benchmark valgrind (based on clang-6.0)
$(OUTPUT_DIR_NAME)/tcc.json:
	$(MAKE) -C $(DOCKER_DIR)/BENCHMARK_VALGRIND run OUTPUT_DIR=$(OUTPUT_DIR) BENCHMARK_RUNS=$(BENCHMARK_RUNS) BENCHMARK_EXEC_ARGS="$(BENCHMARK_EXEC_ARGS)"

#no-papi.json:
#	../tools/bench.py -y --filter-runtime="(gcc|clang)-(O0|O3)" no-papi.json --no-papi --suffix='-no-papi' --runs=${RUNS}

merged.json: gcc.json clang.json compcert.json tcc.json sanitizers.json valgrind.json callgrind.json sulong-warmup.json no-papi.json
	rm -f merged.json
	../tools/merge_results.py gcc.json clang.json compcert.json tcc.json sanitizers.json valgrind.json callgrind.json sulong-warmup.json no-papi.json merged.json

Rplots.pdf: merged.json
	../tools/plot.R --benchfile=merged.json --base-runtime=clang-O0
