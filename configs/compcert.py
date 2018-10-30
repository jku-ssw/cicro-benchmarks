from functools import partial

compcert_kwargs = {'build_system_func': partial(build_system_executor, cc_version='--version', as_version='--version')}

harness.add_runtime('compcert-O', {"CC": "${COMPCERT}", "AS": "as", "CFLAGS": "-fall -O"}, **compcert_kwargs)
harness.add_runtime('compcert-O0', {"CC": "${COMPCERT}", "AS": "as", "CFLAGS": "-fall -O0"}, **compcert_kwargs)
harness.add_runtime('compcert-Os', {"CC": "${COMPCERT}", "AS": "as", "CFLAGS": "-fall -Os"}, **compcert_kwargs)
