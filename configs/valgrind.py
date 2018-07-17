
import subprocess
import json


def execute_binary_analysis_tool(filepath, workdir, tool, exec_args, **kwargs):
    args = tool + [filepath, '--output=json'] + exec_args
    with subprocess.Popen(args, cwd=workdir, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as process:
        stdout, stderr = process.communicate(timeout=kwargs.get('timeout', 240))

        stdout_decoded = stdout.decode('utf-8') if stdout else None
        stderr_decoded = stderr.decode('utf-8') if stderr and process.returncode != 0 else None

        if stdout_decoded:
            try:
                return json.loads(stdout_decoded), stderr_decoded
            except ValueError:
                logger.exception('invalid benchmark result: \'%s\'', stdout_decoded)

        return None, stderr_decoded


def valgrind_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['valgrind', '--error-exitcode=1'], exec_args, **kwargs)


def callgrind_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['valgrind', '--error-exitcode=1', '--tool=callgrind'], exec_args, **kwargs)


def drmemory_executor(filepath, workdir, exec_args, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['{$DR_MEMORY}', '--'], exec_args, **kwargs)  # TODO: exit code?


harness.add_runtime('valgrind-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3", "PAPI": 0}, exec_func=valgrind_executor)
harness.add_runtime('callgrind-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3", "PAPI": 0}, exec_func=callgrind_executor)
harness.add_runtime('drmemory-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"}, exec_func=drmemory_executor)
