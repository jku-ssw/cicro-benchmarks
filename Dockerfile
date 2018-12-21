FROM scratch

# our benchmarking project directories
COPY ./benchmarks   /benchmarks/benchmarks
COPY ./configs      /benchmarks/configs
COPY ./C-Hayai      /benchmarks/C-Hayai
COPY ./tools        /benchmarks/tools
COPY ./CRAN         /benchmarks/CRAN

# our benchmarking project files
COPY ./requirements.R     /benchmarks/requirements.R
