harness.add_runtime('softboundcets-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-O3 -fsoftboundcets -L${SOFTBOUND_RUNTIME_DIR}", "LDFLAGS": "-lm -lrt -lsoftboundcets_rt"})
