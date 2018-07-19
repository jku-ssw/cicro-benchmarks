from functools import partial

tcc_kwargs = {'build_system_func': partial(build_system_executor, cc_version='-v', as_version='-v')}

harness.add_runtime('tcc', {"CC": "${TCC}", "AS": "${TCC}", "LIBRARY_PATH": "/usr/local/lib"}, **tcc_kwargs)
