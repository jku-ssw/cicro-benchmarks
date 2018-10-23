from functools import partial

clang_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}
harness.add_runtime('dmalloc-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "LDFLAGS": "-ldmalloc"}, **clang_kwargs)
