# Benchmark Bitcode files
# What do we want?

* small overhead, no jitter of the actual framework
* supports accurate testcase runtime measurements of about 1µs-10µs?
* no Dead code elimination?
* high performance counters
* count time, instructions, ...?
* runs on linux
* normal ANSI C
* exports measurement points, mean, min, max, quartil, standard-derivation
* visualisation?
* warmup phase detection?
* helper functions



## Setup google-benchmark with wllvm

```bash
export LLVM_COMPILER=clang
export CC=wllvm
export CXX=wllvm++
git clone https://github.com/google/benchmark.git
git clone https://github.com/google/googletest.git benchmark/googletest
mkdir build && cd build
cmake ../benchmark
make
```

Problem: cannot execute tests on lli due to __dso_handling not emitted, not implemented intrinsics

## https://nonius.io/

Dependency to boost

lli:
```
lli ./100-Cranium_test.bc -r html -o results_lli.html -t "Some benchmarks" -s 250
LLVM ERROR: Cannot select: 0x557f840ff6f0: i64 = X86ISD::WrapperRIP TargetGlobalTLSAddress:i64<i8** @_ZSt15__once_callable> 0 [TF=10]
  0x557f840ff4e8: i64 = TargetGlobalTLSAddress<i8** @_ZSt15__once_callable> 0 [TF=10]
In function: _ZSt9call_onceIMNSt13__future_base13_State_baseV2EFvPSt8functionIFSt10unique_ptrINS0_12_Result_baseENS4_8_DeleterEEvEEPbEJPS1_S9_SA_EEvRSt9once_flagOT_DpOT0_
```

sulong:
```
Caused by: java.lang.LinkageError: External function @_ZNSt15basic_streambufIcSt11char_traitsIcEE5imbueERKSt6locale cannot be found.
```

# https://github.com/nickbruun/hayai

lli:
```
LLVM ERROR: Program used external function '__dso_handle' which could not be resolved!
```

sulong:
```
Caused by: java.lang.LinkageError: External function @_ZNSt8ios_base4InitC1Ev cannot be found.
```

# https://github.com/nicopauss/C-Hayai

Works so far in lli and sulong

# https://github.com/QwertyQaz414/quick_benchmark

Seems to be pretty basic without real statistics
