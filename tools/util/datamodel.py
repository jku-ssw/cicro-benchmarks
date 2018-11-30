from sqlalchemy import Column, ForeignKey, Boolean, Integer, Numeric, String, DateTime
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
    datetime = Column(DateTime, nullable=False)
    stderr = Column(String, nullable=True)
    stdout = Column(String, nullable=True)
    exit_code = Column(Integer, nullable=True)

    configuration = relationship("Configuration", back_populates="executions")

    runs = relationship('Run')

    def __repr__(self):
        return '<Execution "{}" configuration={}>'.format(self.id, self.configuration)


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
