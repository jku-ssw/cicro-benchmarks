# Safecode (LLVM 3.7)

## Build Errors

```
CMake Error at cmake/modules/LLVMProcessSources.cmake:83 (message):
  Found unknown source file
  /safecode-llvm37/projects/poolalloc/runtime/PreRT/qsort.c

  Please update
  /safecode-llvm37/projects/poolalloc/runtime/PreRT/CMakeLists.txt

Call Stack (most recent call first):
  cmake/modules/LLVMProcessSources.cmake:54 (llvm_check_source_file_list)
  cmake/modules/AddLLVM.cmake:331 (llvm_process_sources)
  cmake/modules/AddLLVM.cmake:493 (llvm_add_library)
  projects/poolalloc/runtime/PreRT/CMakeLists.txt:2 (add_llvm_library)


CMake Error at cmake/modules/LLVMProcessSources.cmake:83 (message):
  Found unknown source file
  /safecode-llvm37/projects/poolalloc/runtime/PreRT/strdup.c

  Please update
  /safecode-llvm37/projects/poolalloc/runtime/PreRT/CMakeLists.txt

Call Stack (most recent call first):
  cmake/modules/LLVMProcessSources.cmake:54 (llvm_check_source_file_list)
  cmake/modules/AddLLVM.cmake:331 (llvm_process_sources)
  cmake/modules/AddLLVM.cmake:493 (llvm_add_library)
  projects/poolalloc/runtime/PreRT/CMakeLists.txt:2 (add_llvm_library)
```
