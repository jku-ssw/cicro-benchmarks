warmup: sulong-warmup.json

sulong-warmup.json:
	../tools/bench.py -y --filter-runtime gcc-O0 sulong-warmup.json --filter-harness='(?!(faragon-libsrt|armon-libart))'
	../tools/bench.py -y --filter-runtime sulong-warmup-0 sulong-warmup.json --filter-harness='(?!(faragon-libsrt|armon-libart))' --exec-args="--iterations=1" --runs=1
	../tools/bench.py -y --skip-compilation --filter-runtime sulong-warmup-1 sulong-warmup.json --filter-harness='(?!(faragon-libsrt|armon-libart))' --exec-args="--iterations=1" --runs=1
