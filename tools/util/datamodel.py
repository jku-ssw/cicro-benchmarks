from sqlalchemy import Column, ForeignKey, Boolean, Integer, BigInteger, Float, String, DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship

Base = declarative_base()


class Harness(Base):
    __tablename__ = 'harness'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    name = Column(String, unique=True, nullable=False)

    benchmarks = relationship('Benchmark')
    executions = relationship('Execution')

    def __repr__(self):
        return '<Harness name="{}" benchmarks={}>'.format(self.name, self.benchmarks)


class Benchmark(Base):
    __tablename__ = 'benchmark'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    name = Column(String, unique=True, nullable=False)
    harness_id = Column(Integer, ForeignKey('harness.id'), nullable=False)

    harness = relationship("Harness", back_populates="benchmarks", lazy="joined")

    runs = relationship('Run')

    def __repr__(self):
        return '<Benchmark "{}">'.format(self.name)


class Configuration(Base):
    __tablename__ = 'configuration'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    name = Column(String, unique=True, nullable=False)

    executions = relationship('Execution')

    def __repr__(self):
        return '<Configuration "{}">'.format(self.name)


class Execution(Base):
    __tablename__ = 'execution'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    harness_id = Column(Integer, ForeignKey('harness.id'), nullable=False)
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

    harness = relationship("Harness", back_populates="executions", lazy="joined")
    configuration = relationship("Configuration", back_populates="executions", lazy="joined")

    runs = relationship('Run')
    build_system = relationship('ExecutionBuildSystem', lazy="selectin")
    make_env = relationship('ExecutionMakeEnv', lazy="selectin")
    sys_cpu = relationship('ExecutionSystemCpu', order_by="asc(ExecutionSystemCpu.idx)", lazy="selectin")

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
    percent = Column(Float, nullable=False)
    cur_clock = Column(Float, nullable=False)
    min_clock = Column(Float, nullable=False)
    max_clock = Column(Float, nullable=False)

    execution = relationship("Execution", back_populates="sys_cpu")

    def __repr__(self):
        return '<ExecutionSystemCpu "{}-{}" {}%>'.format(self.execution_id, self.idx, self.percent)


class Run(Base):
    __tablename__ = 'run'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    execution_id = Column(Integer, ForeignKey('execution.id'), nullable=False)
    benchmark_id = Column(Integer, ForeignKey('benchmark.id'), nullable=False)

    clock_resolution = Column(Float, nullable=True)
    clock_resolution_measured = Column(Float, nullable=True)
    clock_type = Column(String, nullable=True)
    disabled = Column(Boolean, nullable=False)
    iterations_per_run = Column(Integer, nullable=True)

    benchmark = relationship("Benchmark", back_populates="runs")
    execution = relationship("Execution", back_populates="runs")

    datapoints = relationship('Datapoint', order_by="asc(Datapoint.idx)")

    def __repr__(self):
        return '<Run "{}">'.format(self.id)


class Datapoint(Base):
    __tablename__ = 'datapoint'
    idx = Column(Integer, primary_key=True, nullable=False)
    run_id = Column(Integer, ForeignKey('run.id'), primary_key=True, nullable=False)
    key = Column(String,  primary_key=True, nullable=False)
    value = Column(Float, nullable=False)

    run = relationship("Run", back_populates="datapoints")

    def __repr__(self):
        return '<Datapoint "{}-{}" "{}"="{}">'.format(self.idx, self.run_id, self.key, self.value)
