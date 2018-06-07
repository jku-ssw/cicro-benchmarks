harness.add_runtime('softbound-O3', {"CC": "${SOFTBOUND_CLANG}", "AS": "${SOFTBOUND_CLANG}", "CFLAGS": "-O3 -fsoftboundcets -L${SOFTBOUND_RUNTIME_DIR} -lsoftboundcets_rt", "LDFLAGS": ""})
