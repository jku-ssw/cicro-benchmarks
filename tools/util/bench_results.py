import json
import sqlite3

from .color_logger import get_logger

logger = get_logger('bench_results' )


def get_benchmark_name(benchmark):
    return "{}.{}".format(benchmark.get('fixture'), benchmark.get('name'))

class BenchmarkingResults(object):
    def __init__(self):
        self.conn = sqlite3.connect(':memory:')  # we only need the db at runtime
        self.c = self.conn.cursor()

        self._init_db()

    def _init_db(self):
        create_table = """CREATE TABLE IF NOT EXISTS `BENCHMARKS`(
            `NAME` TEXT NOT NULL,
            `RUNTIME` TEXT NOT NULL,
            `HARNESS` TEXT,
            `DATA` BLOB,
            PRIMARY KEY(`NAME`, `RUNTIME`)
        )"""

        self.conn.execute(create_table)

    def load_file(self, file):
        """Load files which contains the benchmarking data

        data is stored in the following format:

        "benchmark": {
            "runtime" : {
                # json data outputted by harness
            }
        }
        """
        file_data = json.load(file)

        for runtime_data in file_data.values():
            for runtime, data in runtime_data.items():
                self.add_benchmark(data, runtime)

    def store_file(self, file):
        file_data = {}

        for row in self.get_all():
            name = row['name']
            runtime = row['runtime']
            harness = row['harness']
            data = row['data']

            if name not in file_data:
                file_data[name] = {}

            file_data[name][runtime] = data
            file_data[name][runtime]['harness'] = harness

        json.dump(file_data, file)

    def set_single_run(self, harness_name, runtime, data, overwrite=False):
        """if harness_name not in self.data:
            self.data[harness_name] = {}

        if runtime in self.data[harness_name]:
            if overwrite:
                logger.warning('benchmark run of "%s:%s" already present, overwrite data', harness_name, runtime)
            else:
                logger.error('benchmark run of "%s:%s" already present, skip writing', harness_name, runtime)
                logger.info('data: %s', data)
                return
        logger.debug('set benchmark run of "%s:%s"', harness_name, runtime)
        if very_verbose:
            logger.debug('data: "%s"', data)"""

        for benchmark in data.get('benchmarks', []):
            self.add_benchmark(benchmark, runtime, harness_name)

    def get_benchmark(self, benchmark_name, runtime):
        self.c.execute('SELECT `DATA` FROM `BENCHMARKS` WHERE `NAME`=? AND `RUNTIME`=?', (benchmark_name, runtime))
        return self.c.fetchone()  # TODO: return data?

    def is_benchmark_present(self, benchmark_name, runtime):
        return self.get_benchmark(benchmark_name, runtime) is not None

    def add_benchmark(self, data, runtime, harness=None, overwrite=False):
        benchmark_name = get_benchmark_name(data)
        harness = harness if harness is not None else data.get('harness')

        if self.is_benchmark_present(benchmark_name, runtime):
            if not overwrite:
                logger.error('benchmark already present in db: "%s:%s"', benchmark_name, runtime)
                return

            query = """UPDATE `BENCHMARKS` SET `HARNESS`=?,`DATA`=? WHERE `NAME`=? AND `RUNTIME`=?"""
            try:
                self.c.execute(query, (harness, json.dumps(data), benchmark_name, runtime))
                self.conn.commit()
            except sqlite3.IntegrityError as e:
                logger.exception("SQL ERROR: ", e)

        else:
            query = """INSERT INTO `BENCHMARKS` (
                `NAME`,
                `RUNTIME`,
                `HARNESS`,
                `DATA`
            ) VALUES (?, ?, ?, ?)
            """
            try:
                self.c.execute(query, (benchmark_name, runtime, harness, json.dumps(data)))
                self.conn.commit()
            except sqlite3.IntegrityError as e:
                logger.exception("SQL ERROR: ", e)

    def is_run_present(self, harness, runtime):
        self.c.execute('SELECT `NAME` FROM `BENCHMARKS` WHERE `RUNTIME`=? AND `HARNESS`=? LIMIT 1', (runtime, harness))
        return self.c.fetchone() is not None

    def get_all(self):
        self.c.execute('SELECT `NAME`, `RUNTIME`, `HARNESS`, `DATA` FROM `BENCHMARKS` ORDER BY `NAME`')
        return [{'name': result[0], 'runtime': result[1], 'harness': result[2], 'data': json.loads(result[3])} for result in self.c.fetchall()]

    def get_all_benchmarks_of_runtime(self, runtime):
        self.c.execute('SELECT `NAME` FROM `BENCHMARKS` WHERE `RUNTIME`=? ORDER BY `NAME`', (runtime,))
        return [result[0] for result in self.c.fetchall()]

    def get_all_benchmark_names(self):
        self.c.execute('SELECT DISTINCT(`NAME`) FROM `BENCHMARKS` ORDER BY `NAME`')
        return [result[0] for result in self.c.fetchall()]

    def get_all_runtimes(self):
        self.c.execute('SELECT DISTINCT(`RUNTIME`) FROM `BENCHMARKS` ORDER BY `RUNTIME`')
        return [result[0] for result in self.c.fetchall()]

    def get_all_harness(self):
        self.c.execute('SELECT DISTINCT(`HARNESS`) FROM `BENCHMARKS` ORDER BY `HARNESS`')
        return [result[0] for result in self.c.fetchall()]