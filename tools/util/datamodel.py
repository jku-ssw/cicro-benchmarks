from sqlalchemy import Column, ForeignKey, Boolean, Integer, Numeric, String
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

    runs = relationship('Runs')

    def __repr__(self):
        return '<Benchmark "{}">'.format(self.name)


class Configuration(Base):
    __tablename__ = 'configuration'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    harness_name = Column(String, ForeignKey('harness.name'), nullable=False)
    name = Column(String, nullable=False)

    harness = relationship("Harness", back_populates="configurations")

    runs = relationship('Runs')

    def __repr__(self):
        return '<Configuration "{}" harness="{}">'.format(self.id, self.harness.name)


class Runs(Base):
    __tablename__ = 'runs'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    benchmark_name = Column(String, ForeignKey('benchmark.name'), nullable=False)
    configuration_id = Column(Integer, ForeignKey('configuration.id'), nullable=False)

    benchmark = relationship("Benchmark", back_populates="runs")
    configuration = relationship("Configuration", back_populates="runs")

    runs = relationship('Run')

    def __repr__(self):
        return '<Runs "{}" configuration={} benchmark="{}">'.format(self.id, self.configuration,  self.benchmark.name)


class Run(Base):
    __tablename__ = 'run'
    id = Column(Integer, primary_key=True, nullable=False, index=True)
    runs_id = Column(Integer, ForeignKey('runs.id'), nullable=False)

    clock_resolution = Column(Numeric, nullable=True)
    clock_resolution_measured = Column(Numeric, nullable=True)
    clock_type = Column(String, nullable=True)
    disabled = Column(Boolean, nullable=False)
    iterations_per_run = Column(Integer, nullable=True)

    runs = relationship("Runs", back_populates="runs")

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
