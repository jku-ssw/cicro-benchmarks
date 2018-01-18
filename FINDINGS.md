# swenston/sort (2017.24.11)

```
export LLVM_COMPILER=clang
CC=wllvm make multidemo
extract-bc multidemo
cp multidemo.bc ../../../java-llvm-ir-builder-dev/sulong/multidemo.bc
```

## Execution on sulong:

```
time mx --jdk jvmci --dynamicimports=/compiler lli multidemo.bc  -Dgraal.TraceTruffleCompilation=true -Dgraal.TraceTruffleCompilation=true
```

Shows lots of opt invalidated even when the program runs > 1 minute, which is not correct for a program with only does the same

```
[truffle] opt done         @sorter_quick_sort_recursive <opt>                          |ASTSize     839/  839 |Time    51(  45+6   )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress 0x7f0aea4acd90 |Source     sort.h:696 
[truffle] opt done         @sorter_quick_sort_recursive <opt>                          |ASTSize     839/  839 |Time    49(  42+7   )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress 0x7f0aea59a890 |Source     sort.h:696 
[truffle] opt invalidated  @run_tests                                                  |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @run_tests                                                  |ASTSize    1458/ 2128 |Time   254( 130+124 )ms |DirectCallNodes I   28/D    3 |GraalNodes  3818/ 4443 |CodeSize        13729 |CodeAddress       0x0 |Source multidemo.c:127 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    16(  13+2   )ms |DirectCallNodes I    0/D    1 |GraalNodes    28/   31 |CodeSize          176 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    16(  15+2   )ms |DirectCallNodes I    0/D    1 |GraalNodes    28/   31 |CodeSize          176 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    51(  44+7   )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @run_tests                                                  |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @run_tests                                                  |ASTSize    1458/ 2128 |Time   263( 143+120 )ms |DirectCallNodes I   28/D    3 |GraalNodes  3818/ 4443 |CodeSize        13729 |CodeAddress       0x0 |Source multidemo.c:127 
[truffle] opt done         @sorter_quick_sort_recursive <opt>                          |ASTSize     839/  839 |Time    16(  15+1   )ms |DirectCallNodes I    0/D    1 |GraalNodes    28/   31 |CodeSize          176 |CodeAddress 0x7f0ae9dd6a90 |Source     sort.h:696 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    49(  40+9   )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    40(  33+7   )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    47(  35+12  )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @run_tests                                                  |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @run_tests                                                  |ASTSize    1458/ 2128 |Time   243( 125+117 )ms |DirectCallNodes I   28/D    3 |GraalNodes  3818/ 4443 |CodeSize        13729 |CodeAddress       0x0 |Source multidemo.c:127 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    15(  13+2   )ms |DirectCallNodes I    0/D    1 |GraalNodes    28/   31 |CodeSize          176 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    45(  35+10  )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt done         @sorter_quick_sort_recursive <opt>                          |ASTSize     839/  839 |Time    46(  37+9   )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress 0x7f0ae96a9590 |Source     sort.h:696 
[truffle] opt done         @sorter_quick_sort_recursive <opt>                          |ASTSize     839/  839 |Time    49(  39+10  )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress 0x7f0ae9a41950 |Source     sort.h:696 
[truffle] opt invalidated  @run_tests                                                  |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @run_tests                                                  |ASTSize    1458/ 2128 |Time   275( 137+138 )ms |DirectCallNodes I   28/D    3 |GraalNodes  3818/ 4443 |CodeSize        13729 |CodeAddress       0x0 |Source multidemo.c:127 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    43(  34+9   )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @sorter_quick_sort_recursive                                |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @sorter_quick_sort_recursive                                |ASTSize     839/  839 |Time    50(  38+12  )ms |DirectCallNodes I    0/D    1 |GraalNodes   409/  400 |CodeSize         1118 |CodeAddress       0x0 |Source     sort.h:696 
[truffle] opt invalidated  @run_tests                                                  |SourceClass OptimizedAssumption |Source Assumption(invalid, name=null) |Reason assumption already invalidated when installing code 
[truffle] opt done         @run_tests                                                  |ASTSize    1458/ 2128 |Time   271( 151+121 )ms |DirectCallNodes I   28/D    3 |GraalNodes  3818/ 4443 |CodeSize        13729 |CodeAddress       0x0 |Source multidemo.c:127
```

## Patch (not cleaned up)

```diff
diff --git a/multidemo.c b/multidemo.c
index 90e8316..4fdf8bf 100644
--- a/multidemo.c
+++ b/multidemo.c
@@ -59,8 +59,8 @@
 
 /* Used to control the demo */
 #define SEED 123
-#define SIZE 10000
-#define RUNS 1
+#define SIZE 400
+#define RUNS 20
 
 /* helper functions */
 void verify(int64_t *dst, const int size) {
@@ -131,79 +131,101 @@ void run_tests(void) {
   double start_time;
   double end_time;
   double total_time;
-  printf("Running tests\n");
+  //printf("Running tests\n");
   srand48(SEED);
-  total_time = 0.0;
-
+  //total_time = 0.0;
+start_time = utime();
   for (i = 0; i < RUNS; i++) {
     fill(arr, SIZE);
     memcpy(dst, arr, sizeof(int64_t) * SIZE);
-    start_time = utime();
+    
     qsort(dst, SIZE, sizeof(int64_t), simple_cmp);
-    end_time = utime();
-    total_time += end_time - start_time;
+    
+    
     verify(dst, SIZE);
   }
-
-  printf("stdlib qsort time:          %10.2f us per iteration\n", total_time / RUNS);
+//end_time = utime();
+//total_time += end_time - start_time;
+ // printf("stdlib qsort time:          %10.2f us per iteration\n", total_time / RUNS);
 #if !defined(__linux__) && !defined(__CYGWIN__)
-  srand48(SEED);
-  total_time = 0.0;
 
+
+  srand48(SEED);
+  //total_time = 0.0;
+//start_time = utime();
   for (i = 0; i < RUNS; i++) {
     fill(arr, SIZE);
     memcpy(dst, arr, sizeof(int64_t) * SIZE);
-    start_time = utime();
+    
     heapsort(dst, SIZE, sizeof(int64_t), simple_cmp);
-    end_time = utime();
-    total_time += end_time - start_time;
+    
     verify(dst, SIZE);
   }
+//end_time = utime();
+   // total_time += end_time - start_time;
+ // printf("stdlib heapsort time:       %10.2f us per iteration\n", total_time / RUNS);
 
-  printf("stdlib heapsort time:       %10.2f us per iteration\n", total_time / RUNS);
   srand48(SEED);
-  total_time = 0.0;
+  //total_time = 0.0;
+
+//start_time = utime();
 
   for (i = 0; i < RUNS; i++) {
     fill(arr, SIZE);
     memcpy(dst, arr, sizeof(int64_t) * SIZE);
-    start_time = utime();
+    
     mergesort(dst, SIZE, sizeof(int64_t), simple_cmp);
-    end_time = utime();
-    total_time += end_time - start_time;
+    
     verify(dst, SIZE);
   }
-
-  printf("stdlib mergesort time:      %10.2f us per iteration\n", total_time / RUNS);
+//end_time = utime();
+    //total_time += end_time - start_time;
+  //printf("stdlib mergesort time:      %10.2f us per iteration\n", total_time / RUNS);
 #endif
   srand48(SEED);
-  total_time = 0.0;
-
+  //total_time = 0.0;
+//start_time = utime();
   for (i = 0; i < RUNS; i++) {
     fill(arr, SIZE);
     memcpy(dst, arr, sizeof(int64_t) * SIZE);
-    start_time = utime();
+    
     sorter_quick_sort(dst, SIZE);
-    end_time = utime();
-    total_time += end_time - start_time;
+
     verify(dst, SIZE);
   }
-
-  printf("quick sort time:            %10.2f us per iteration\n", total_time / RUNS);
+    //end_time = utime();
+    //total_time += end_time - start_time;
+  //printf("quick sort time:            %10.2f us per iteration\n", total_time / RUNS);
   srand48(SEED);
   total_time = 0.0;
 
   for (i = 0; i < RUNS; i++) {
     fill(arr, SIZE);
     memcpy(dst, arr, sizeof(int64_t) * SIZE);
-    start_time = utime();
+    //start_time = utime();
     sorter_selection_sort(dst, SIZE);
-    end_time = utime();
-    total_time += end_time - start_time;
+    //end_time = utime();
+    //total_time += end_time - start_time;
     verify(dst, SIZE);
   }
 
-  printf("selection sort time:        %10.2f us per iteration\n", total_time / RUNS);
+  //printf("selection sort time:        %10.2f us per iteration\n", total_time / RUNS);
+
+  //total_time = 0.0;
+//start_time = utime();
+  for (i = 0; i < RUNS; i++) {
+    fill(arr, SIZE);
+    memcpy(dst, arr, sizeof(int64_t) * SIZE);
+    
+    sorter_selection_sort(dst, SIZE);
+
+    verify(dst, SIZE);
+  }
+   // end_time = utime();
+   // total_time += end_time - start_time;
+  //printf("selection sort time2:        %10.2f us per iteration\n", total_time / RUNS);
+
+  return;
   srand48(SEED);
   total_time = 0.0;
   
@@ -455,8 +477,27 @@ void run_tests2(void) {
 
   printf("tim sort time:              %10.2f us per iteration\n", total_time / RUNS);
 }
+
+void my_test() {
+    for(int i=0; i < 200; i++) {
+      run_tests();
+
+      // run_tests2();
+  }
+}
+
 int main(void) {
-  run_tests();
-  run_tests2();
+  for(int i=0; i < 100; i++) {
+      my_test();
+      printf(".");
+      fflush(stdout);
+      //run_tests2();
+  }
+  double start_time = utime();
+  my_test();
+  //run_tests2();
+  double end_time = utime();
+  printf("\n######################################\n");
+  printf("time testsuite:              %10.2f us\n", end_time - start_time);
   return 0;
 }
```


# rigtorp/ipc-bench (2017.30.11)

```
export LLVM_COMPILER=clang
CC=wllvm make
extract-bc tcp_lat
cp tcp_lat.bc ../../../java-llvm-ir-builder-dev/sulong/tcp_lat.bc
```

command uses fork -> not usable in sulong yet
benchmark questionable, because it's mainly targeted at the host system


# nitlang/nit (2017.30.11)

```
sudo pacman -S ccache

export LLVM_COMPILER=clang
CC=wllvm make
```

Some problems with building the benchmarks


# wbhart/bsdnt (2017.30.11)

```
export LLVM_COMPILER=clang
./configure
make AS='wllvm' CC='wllvm'
extract-bc build/test/t-nn_linear
cp build/test/t-nn_linear.bc ../../../java-llvm-ir-builder-dev/sulong/t-nn_linear.bc
```

Similar bug found as for: swenston/sort

Bugreport: https://github.com/graalvm/sulong/issues/824


# antirez/otree (2017.30.11)

```
export LLVM_COMPILER=clang
wllvm -o btree_example btree.c btree_example.c -Wall -g -rdynamic -ggdb -O2  # -W causes wllvm to not work?
extract-bc btree_example
cp btree_example.bc ../../../java-llvm-ir-builder-dev/sulong/btree_example.bc
```

```
time mx --jdk jvmci --dynamicimports=/compiler lli btree_example.bc allocfree 10000000000 200000 -Dgraal.TraceTruffleCompilation=true -Dgraal.TraceTruffleCompilation=true
```

# chrismoos/hash-ring (2017.30.11)

```
sudo pacman -S erlang  # for test script (didn't worked)
```

# zhemao/libds (2017.30.11)

```
export LLVM_COMPILER=clang
make CC='wllvm' test
extract-bc vectest
cp vectest.bc ../../../java-llvm-ir-builder-dev/sulong/vectest.bc
```

# openSUSE/libsolv (2017.30.11)

```
cmake .
make test # TODO: continue
```

# dcjones/hat-trie (2017.15.12)

```
autoreconf -i
./configure
export LLVM_COMPILER=clang
make CC='wllvm' check
```

# Ed-von-Schleck/shoco (2017.18.12)

does not build due to syntax problems

# chrismoos/hash-ring (2017.18.12)

unclear which code to use

# cvxgrp/scs (2017.18.12)

unclear how to use code as benchmark

# PetteriAimonen/libfixmatrix (2017.18.12)

svn dependency not available

# andikleen/snappy-c (2017.18.12)

Some valgrind warnings:

```
==19718== Conditional jump or move depends on uninitialised value(s)
==19718==    at 0x10B3BA: decompress_all_tags (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109F21: internal_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109E66: snappy_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1096B1: test_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1093BC: do_benchmark (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109086: _execute_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109756: main (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718== 
==19718== Conditional jump or move depends on uninitialised value(s)
==19718==    at 0x10BAEA: writer_try_fast_append (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x10B3EF: decompress_all_tags (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109F21: internal_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109E66: snappy_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1096B1: test_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1093BC: do_benchmark (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109086: _execute_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109756: main (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718== 
==19718== Conditional jump or move depends on uninitialised value(s)
==19718==    at 0x10B49C: decompress_all_tags (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109F21: internal_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109E66: snappy_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1096B1: test_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1093BC: do_benchmark (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109086: _execute_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109756: main (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718== 
==19718== Conditional jump or move depends on uninitialised value(s)
==19718==    at 0x10B4F7: decompress_all_tags (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109F21: internal_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109E66: snappy_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1096B1: test_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1093BC: do_benchmark (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109086: _execute_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109756: main (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718== 
==19718== Conditional jump or move depends on uninitialised value(s)
==19718==    at 0x10BBDF: writer_append (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x10B598: decompress_all_tags (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109F21: internal_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109E66: snappy_uncompress (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1096B1: test_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x1093BC: do_benchmark (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109086: _execute_harness (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718==    by 0x109756: main (in /home/thomas/JKU/ssw_msr2018/make_test/andikleen-snappy-c_test)
==19718== 
```

# https://github.com/josephg/librope (2017.18.12)

code in README.md uses function names which no longer exists

memleak in example snippet

# https://github.com/veorq/SipHash (2017.19.12)

implemented

# https://github.com/troydhanson/tpl (2017.19.12)

implemented

# https://github.com/snowballstem/snowball (2017.19.12)

Implemented, but way to tight loop at the moment to do good benchmarking

# https://github.com/bashrc/libdeep (2017.19.12)

Implemented

# https://github.com/haldean/x6502 (2017.19.12)

Dropped: We need an 6502 assembler to build files which can be run

# https://github.com/camgunz/cmp (2017.19.12)

Implemented, but way to tight loop at the moment to do good benchmarking

# https://github.com/jtsiomb/kdtree (2017.20.12)

implemented

# https://github.com/ferreiradaselva/mathc (2017.20.12)

Example code in readme doesn't work out of the box

Implemented, but way to tight loop at the moment to do good benchmarking

# https://github.com/ctz/cifra (2017.20.12)

Implement SHA3 Benchmark

# https://github.com/khovratovich/Argon2 (2017.20.12)

Implemented

# https://github.com/codeplea/genann (2017.20.12)

Implemented

# https://github.com/y-256/libdivsufsort (2017.21.12)

Dropped because of cmake dependency and we cannot compile without it

# https://github.com/websnarf/bstrlib (2017.21.12)

Implemented

# https://github.com/faragon/libsrt (2017.22.12)

Implemented

# https://github.com/RoaringBitmap/CRoaring (2017.22.12)

Implemented

# https://github.com/Themaister/libfmsynth (2017.22.12)

Implemented

# https://github.com/chrismoos/hash-ring (2017.22.12)

Implemented

# https://github.com/zserge/partcl (2017.22.12)

Implemented

this testcase caused a segfault:
```tcl
set i 0; while {< $i 20} {if {== [- $i [* [/ $i 2] 2]] 0} {set i [+ $i 1]} {set i [+ $i 1]}; puts $i}
```

# https://github.com/sahaRatul/sela (2017.22.12)

Code seems to be mainly constructed of main functions

# https://github.com/hfp/libxsmm (2018.01.11)

Contains fortran code, is not easy to include

# https://github.com/kylophone/spectrophoto (2018.01.11)

Implemented

# https://github.com/liteserver/binn (2018.01.11)

Implemented

# https://github.com/chrisjmccormick/word2vec_commented (2018.01.11)
# https://github.com/tmikolov/word2vec (2018.01.11)

Dropped, because constructed of main functions

# https://github.com/gingerBill/Odin (2018.01.11)

Dependency on LLVM and clang

# https://github.com/openSUSE/libsolv (2018.01.11)

complexity of the the projects make it hard to write tests

# https://github.com/watmough/jwHash (2018.01.12)

Project is broken for now: https://github.com/watmough/jwHash/pull/6

# https://github.com/lemire/simdcomp (2018.01.12)

Implemented

# https://github.com/zedshaw/liblcthw (2018.01.12)

sarray.h seems to have quite some problems with the qsort_r implementation

bstree.h is now implemented

# https://github.com/yosefk/checkedthreads (2018.01.12)

Implemented

Note: There is a memleak when ct_init() and ct_fini() are called again

# https://github.com/jordansissel/grok (2018.01.12)

Dropped, Dependency on tcutil.h

# https://github.com/tkengo/highway (2018.01.18)

Implemented test on dependency library

# https://github.com/parallella/pal (2018.01.18)

Code does not really compile:

```
configure: error: source directory already configured; run "make distclean" there first
configure: error:  failed for /usr/bin
make: *** [Makefile:5130: config.status] Fehler 1
```

```
parallella-pal/src/base/p_broadcast.c:7:9: error: conflicting types for 'p_broadcast'
ssize_t p_broadcast(p_mem_t *mlist[], void *src, size_t nsrc, size_t ndst,
        ^
parallella-pal/include/pal_base.h:211:9: note: previous declaration is here
ssize_t p_broadcast(p_mem_t **mlist[], int mcount, void *src, size_t nb,
```

# https://github.com/msteinbeck/tinyspline (2018.01.18)

Implemented

# https://github.com/tboox/vm86 (2018.01.18)

Dropped: 32bit only, long chain of prefix.h and unclear working of internals
(libc replacement with different method names but same file names, unclear include structure,...)

```
tboox-vm86_$(TEST_SUFFIX): tboox-vm86 tboox-vm86_test.c $(HARNESS_C)
	$(CC)  -m32 $(HARNESS_C) tboox-vm86_test.c tboox-vm86/src/vm86/*.c -o "$(TEST_DIR)/tboox-vm86_$(TEST_SUFFIX)" -I tboox-vm86/src/vm86 -I includes/tboox-vm86 -I tboox-vm86/pkg/tbox.pkg/inc -I tboox-vm86/pkg/tbox.pkg/inc/linux
```

# https://github.com/lacker/ikalman (2018.01.18)

Implemented
