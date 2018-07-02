
import subprocess
import json

def execute_binary_analysis_tool(filepath, workdir, tool, **kwargs):
    args = tool + [filepath, '--output=json'] + kwargs.get('exec_args', '').split(' ')
    with subprocess.Popen(args, cwd=workdir, stdout=subprocess.PIPE) as process:
        stdout, _ = process.communicate(timeout=kwargs.get('timeout', 240))

        if process.returncode != 0:
            return None

        try:
            return json.loads(stdout.decode('utf-8'))
        except json.JSONDecodeError:
            logger.error('invalid benchmark result: \'%s\'', stdout.decode('utf-8'))
            raise

def valgrind_executor(filepath, workdir, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['valgrind'], **kwargs)

def callgrind_executor(filepath, workdir, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['valgrind', '--tool=callgrind'], **kwargs)

def drmemory_executor(filepath, workdir, **kwargs):
    return execute_binary_analysis_tool(filepath, workdir, ['{$DR_MEMORY}', '--'], **kwargs)

harness.add_runtime('valgrind-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3", "PAPI":0}, exec_func=valgrind_executor)
harness.add_runtime('callgrind-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3", "PAPI":0}, exec_func=callgrind_executor)
harness.add_runtime('drmemory-O3', {"CC": "${CLANG}", "AS": "${CLANG}", "CFLAGS": "-Wno-everything -O3"}, exec_func=drmemory_executor)
