def lli_executor(filepath, workdir, **kwargs):
    return wllvm_executor(filepath, workdir, '${LLI}', **kwargs)

harness.add_runtime('lli-O3', {"CC": "wllvm", "AS": "wllvm", "CFLAGS": "-Wno-everything -O3"}, make_func=wllvm_make, exec_func=lli_executor)
