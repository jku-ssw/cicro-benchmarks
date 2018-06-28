import json
import sqlite3

from .color_logger import get_logger

logger = get_logger('bench_results')


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
            `RUN_ID` INTEGER NOT NULL,
            `HARNESS` TEXT,
            `DATA` BLOB,
            PRIMARY KEY(`NAME`, `RUNTIME`, `RUN_ID`)
        )"""

        self.conn.execute(create_table)

    def load_file(self, file, append=False, verbose=False):
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
                if type(data) is not list:
                    data = [data]  # old file structure

                run_id = 0
                for entry in data:
                    if append:
                        self.append_benchmark(entry, runtime)
                    else:
                        self.add_benchmark(entry, runtime, overwrite=False, verbose=verbose, run_id=run_id)
                    run_id += 1

    def store_file(self, file):
        file_data = {}

        for row in self.get_all():
            name = row['name']
            runtime = row['runtime']
            harness = row['harness']
            data = row['data']

            if name not in file_data:
                file_data[name] = {}

            if runtime not in file_data[name]:
                file_data[name][runtime] = []

            data['harness'] = harness
            file_data[name][runtime].append(data)

        json.dump(file_data, file)

    def get_benchmark(self, benchmark_name, runtime, run_id=0):
        self.c.execute('SELECT `DATA` FROM `BENCHMARKS` WHERE `NAME`=? AND `RUNTIME`=? AND `RUN_ID`=?',
                       (benchmark_name, runtime, run_id))
        return self.c.fetchone()  # TODO: return data?

    def is_benchmark_present(self, benchmark_name, runtime, run_id=0):
        return self.get_benchmark(benchmark_name, runtime, run_id) is not None

    def set_single_run(self, harness_name, runtime, data, overwrite=False, run_id=0):
        for benchmark in data.get('benchmarks', []):
            self.add_benchmark(benchmark, runtime, harness_name, overwrite=overwrite, run_id=run_id)

    def append_single_run(self, harness_name, runtime, data):
        for benchmark in data.get('benchmarks', []):
            self.append_benchmark(benchmark, runtime, harness_name)

    def add_benchmark(self, data, runtime, harness=None, overwrite=False, verbose=True, run_id=0):
        bench_name = get_benchmark_name(data)
        harness = harness if harness is not None else data.get('harness')

        if self.is_benchmark_present(bench_name, runtime, run_id):
            if not overwrite:
                logger.error('benchmark run of "%s:%s" already present, skip writing', bench_name, runtime)
                if verbose:
                    logger.info('data: %s', data)
                else:
                    logger.debug('data: %s', data)
                return

            logger.warning('benchmark run of "%s:%s" already present, overwrite data', bench_name, runtime)

            query = """UPDATE `BENCHMARKS` SET `HARNESS`=?,`DATA`=? WHERE `NAME`=? AND `RUNTIME`=?"""
            try:
                self.c.execute(query, (harness, json.dumps(data), bench_name, runtime))
                self.conn.commit()
            except sqlite3.IntegrityError as e:
                logger.exception("SQL ERROR: ", e)

        else:
            query = """INSERT INTO `BENCHMARKS` (
                `NAME`,
                `RUNTIME`,
                `RUN_ID`,
                `HARNESS`,
                `DATA`
            ) VALUES (?, ?, ?, ?, ?)
            """
            try:
                self.c.execute(query, (bench_name, runtime, run_id, harness, json.dumps(data)))
                self.conn.commit()
            except sqlite3.IntegrityError as e:
                logger.exception("SQL ERROR: ", e)

    def append_benchmark(self, data, runtime, harness=None):
        bench_name = get_benchmark_name(data)
        harness = harness if harness is not None else data.get('harness')

        query = """INSERT INTO `BENCHMARKS` (
            `NAME`,
            `RUNTIME`,
            `RUN_ID`,
            `HARNESS`,
            `DATA`
        ) VALUES (?, ?, (SELECT COALESCE(MAX(`RUN_ID`)+1, 0) FROM `BENCHMARKS` WHERE `NAME`=? AND `RUNTIME`=?), ?, ?)
        """
        try:
            self.c.execute(query, (bench_name, runtime, bench_name, runtime, harness, json.dumps(data)))
            self.conn.commit()
        except sqlite3.IntegrityError as e:
            logger.exception("SQL ERROR: ", e)

    def is_run_present(self, harness, runtime, run_id=0):
        self.c.execute('SELECT `NAME` FROM `BENCHMARKS` WHERE `RUNTIME`=? AND `HARNESS`=? AND `RUN_ID`=? LIMIT 1',
                       (runtime, harness, run_id))
        return self.c.fetchone() is not None

    def remove_all_runs(self, harness, runtime):
        logger.debug('remove all runs of "%s:%s"', harness, runtime)
        self.c.execute('DELETE FROM `BENCHMARKS` WHERE `RUNTIME`=? AND `HARNESS`=?', (runtime, harness))

    def get_all(self):
        self.c.execute('SELECT `NAME`, `RUNTIME`, `RUN_ID`, `HARNESS`, `DATA` FROM `BENCHMARKS` ORDER BY `NAME`')
        return [{'name': result[0],
                 'runtime': result[1],
                 'run_id': result[2],
                 'harness': result[3],
                 'data': json.loads(result[4])} for result in self.c.fetchall()]

    def get_all_benchmarks_of_runtime(self, runtime):
        self.c.execute('SELECT `NAME` FROM `BENCHMARKS` WHERE `RUNTIME`=? ORDER BY `NAME`', (runtime,))
        return [result[0] for result in self.c.fetchall()]

    def get_all_benchmark_runs(self, benchmark):
        self.c.execute('SELECT `RUNTIME`, `RUN_ID`, `DATA` FROM `BENCHMARKS` WHERE `NAME`=? ORDER BY `RUNTIME`',
                       (benchmark,))
        result = {}
        for row in self.c.fetchall():
            if row[0] not in result:
                result[row[0]] = []
            result[row[0]].append(json.loads(row[2]))
        return result

    def get_all_benchmark_names(self):
        self.c.execute('SELECT DISTINCT(`NAME`) FROM `BENCHMARKS` ORDER BY `NAME`')
        return [result[0] for result in self.c.fetchall()]

    def get_missing_benchmark_names(self, runtime):
        self.c.execute("""SELECT DISTINCT(`NAME`) FROM `BENCHMARKS`
            WHERE `NAME` NOT IN (SELECT `NAME` FROM `BENCHMARKS` WHERE `RUNTIME`=?) ORDER BY `NAME`""", (runtime, ))
        return [result[0] for result in self.c.fetchall()]

    def get_all_runtimes(self):
        self.c.execute('SELECT DISTINCT(`RUNTIME`) FROM `BENCHMARKS` ORDER BY `RUNTIME`')
        return [result[0] for result in self.c.fetchall()]

    def get_all_harness(self):
        self.c.execute('SELECT DISTINCT(`HARNESS`) FROM `BENCHMARKS` ORDER BY `HARNESS`')
        return [result[0] for result in self.c.fetchall()]
