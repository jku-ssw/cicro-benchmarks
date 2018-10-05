from functools import partial

compcert_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}

harness.add_runtime('compcert', {"CC": "${COMPCERT}", "AS": "as", "CFLAGS": "-fall"}, **compcert_kwargs)
