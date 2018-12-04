from sqlalchemy import Column, ForeignKey, Boolean, Integer, BigInteger, Numeric, String, DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship

Base = declarative_base()


class Harness(Base):
    __tablename__ = 'harness'
    name = Column(String, primary_key=True, nullable=False, index=True)

    benchmarks = relationship('Benchmark')
    configurations = relationship('Configuration')

    def __repr__(self):
        return '<Harness name="{}" benchmarks={}>'.format(self.name, self.benchmarks)


class Benchmark(Base):
    __tablename__ = 'benchmark'
    name = Column(String, primary_key=True, nullable=False, index=True)
    harness_name = Column(String, ForeignKey('harness.name'), nullable=False)

    harness = relationship("Harness", back_populates="benchmarks")

    runs = relationship('Run')

    def __repr__(self):
        return '<Benchmark "{}">'.format(self.name)


class Configuration(Base):
    __tablename__ = 'configuration'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    harness_name = Column(String, ForeignKey('harness.name'), nullable=False)
    name = Column(String, nullable=False)

    harness = relationship("Harness", back_populates="configurations")

    executions = relationship('Execution')

    def __repr__(self):
        return '<Configuration "{}" harness="{}">'.format(self.id, self.harness.name)


class Execution(Base):
    __tablename__ = 'execution'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    configuration_id = Column(Integer, ForeignKey('configuration.id'), nullable=False)
    datetime = Column(DateTime, nullable=True)
    stderr = Column(String, nullable=True)
    stdout = Column(String, nullable=True)
    exit_code = Column(Integer, nullable=True)

    sys_platform = Column(String, nullable=True)
    sys_mem_avail = Column(BigInteger, nullable=True)
    sys_mem_free = Column(BigInteger, nullable=True)
    sys_mem_total = Column(BigInteger, nullable=True)
    sys_mem_used = Column(BigInteger, nullable=True)
    sys_cpu_logical = Column(Integer, nullable=True)
    sys_cpu_physical = Column(Integer, nullable=True)

    configuration = relationship("Configuration", back_populates="executions")

    runs = relationship('Run')
    build_system = relationship('ExecutionBuildSystem')
    make_env = relationship('ExecutionMakeEnv')
    sys_cpu = relationship('ExecutionSystemCpu')

    def __repr__(self):
        return '<Execution "{}" configuration={}>'.format(self.id, self.configuration)


class ExecutionBuildSystem(Base):
    __tablename__ = 'execution_build_system'
    execution_id = Column(Integer, ForeignKey('execution.id'), primary_key=True, nullable=False)
    key = Column(String,  primary_key=True, nullable=False)
    value = Column(String, nullable=False)

    execution = relationship("Execution", back_populates="build_system")

    def __repr__(self):
        return '<ExecutionBuildSystem "{}" "{}"="{}">'.format(self.execution_id, self.key, self.value)


class ExecutionMakeEnv(Base):
    __tablename__ = 'execution_make_env'
    execution_id = Column(Integer, ForeignKey('execution.id'), primary_key=True, nullable=False)
    key = Column(String,  primary_key=True, nullable=False)
    value = Column(String, nullable=False)

    execution = relationship("Execution", back_populates="make_env")

    def __repr__(self):
        return '<ExecutionMakeEnv "{}" "{}"="{}">'.format(self.execution_id, self.key, self.value)


class ExecutionSystemCpu(Base):
    __tablename__ = 'execution_system_cpu'
    execution_id = Column(Integer, ForeignKey('execution.id'), primary_key=True, nullable=False)
    idx = Column(Integer,  primary_key=True, nullable=False)
    percent = Column(Numeric, nullable=False)
    cur_clock = Column(Numeric, nullable=False)
    min_clock = Column(Numeric, nullable=False)
    max_clock = Column(Numeric, nullable=False)

    execution = relationship("Execution", back_populates="sys_cpu")

    def __repr__(self):
        return '<ExecutionSystemCpu "{}-{}" {}%>'.format(self.execution_id, self.idx, self.percent)


class Run(Base):
    __tablename__ = 'run'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    execution_id = Column(Integer, ForeignKey('execution.id'), nullable=False)
    benchmark_name = Column(String, ForeignKey('benchmark.name'), nullable=False)

    clock_resolution = Column(Numeric, nullable=True)
    clock_resolution_measured = Column(Numeric, nullable=True)
    clock_type = Column(String, nullable=True)
    disabled = Column(Boolean, nullable=False)
    iterations_per_run = Column(Integer, nullable=True)

    benchmark = relationship("Benchmark", back_populates="runs")
    execution = relationship("Execution", back_populates="runs")

    datapoints = relationship('Datapoint')

    def __repr__(self):
        return '<Run "{}">'.format(self.id)


class Datapoint(Base):
    __tablename__ = 'datapoint'
    idx = Column(Integer, primary_key=True, nullable=False)
    run_id = Column(Integer, ForeignKey('run.id'), primary_key=True, nullable=False)
    key = Column(String,  primary_key=True, nullable=False)
    value = Column(Numeric, nullable=False)

    run = relationship("Run", back_populates="datapoints")

    def __repr__(self):
        return '<Datapoint "{}-{}" "{}"="{}">'.format(self.idx, self.run_id, self.key, self.value)