# Safeinit

## Build Errors

```
/safeinit/llvm/projects/compiler-rt/lib/sanitizer_common/sanitizer_stoptheworld_linux_libcdep.cc: In function 'int __sanitizer::TracerThread(void*)':
/safeinit/llvm/projects/compiler-rt/lib/sanitizer_common/sanitizer_stoptheworld_linux_libcdep.cc:276:22: error: aggregate 'sigaltstack handler_stack' has incomplete type and cannot be defined
   struct sigaltstack handler_stack;
                      ^~~~~~~~~~~~~
projects/compiler-rt/lib/sanitizer_common/CMakeFiles/RTSanitizerCommonLibc.x86_64.dir/build.make:206: recipe for target 'projects/compiler-rt/lib/sanitizer_common/CMakeFiles/RTSanitizerCommonLibc.x86_64.dir/sanitizer_stoptheworld_linux_libcdep.cc.o' failed
make[2]: *** [projects/compiler-rt/lib/sanitizer_common/CMakeFiles/RTSanitizerCommonLibc.x86_64.dir/sanitizer_stoptheworld_linux_libcdep.cc.o] Error 1
make[1]: *** [projects/compiler-rt/lib/sanitizer_common/CMakeFiles/RTSanitizerCommonLibc.x86_64.dir/all] Error 2
CMakeFiles/Makefile2:12720: recipe for target 'projects/compiler-rt/lib/sanitizer_common/CMakeFiles/RTSanitizerCommonLibc.x86_64.dir/all' failed
```
