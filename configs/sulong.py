def sulong_executor(filepath, workdir, **kwargs):
    return wllvm_executor(filepath, workdir, '$GRAALVM_DIR/lli', **kwargs)

harness.add_runtime('sulong-O1', {"CC": "$(WLLVM_DIR)/wllvm", "AS": "$(WLLVM_DIR)/wllvm", "CFLAGS": "-Wno-everything -O1"}, make_func=wllvm_make, exec_func=sulong_executor)
