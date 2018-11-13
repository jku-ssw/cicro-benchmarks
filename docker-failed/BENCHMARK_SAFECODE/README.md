# Safecode (LLVM 3.2)

## Build Errors

```
llvm[0]: Constructing LLVMBuild project information.
make[1]: Entering directory '/safecode/llvm/lib/Support'
llvm[1]: Compiling APFloat.cpp for Debug+Asserts build
llvm[1]: Compiling APInt.cpp for Debug+Asserts build
llvm[1]: Compiling APSInt.cpp for Debug+Asserts build
llvm[1]: Compiling Allocator.cpp for Debug+Asserts build
In file included from APInt.cpp:16:
In file included from /safecode/llvm/include/llvm/ADT/APInt.h:18:
In file included from /safecode/llvm/include/llvm/ADT/ArrayRef.h:13:
In file included from /safecode/llvm/include/llvm/ADT/SmallVector.h:17:
/safecode/llvm/include/llvm/Support/AlignOf.h:90:1: error: 'alignas' attribute cannot be applied to types
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(1);
^
/safecode/llvm/include/llvm/Support/AlignOf.h:79:10: note: expanded from macro 'LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT'
    char alignas(x) aligned; \
         ^
/safecode/llvm/include/llvm/Support/AlignOf.h:91:1: error: 'alignas' attribute cannot be applied to types
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(2);
^
/safecode/llvm/include/llvm/Support/AlignOf.h:79:10: note: expanded from macro 'LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT'
    char alignas(x) aligned; \
         ^
/safecode/llvm/include/llvm/Support/AlignOf.h:92:1: error: 'alignas' attribute cannot be applied to types
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(4);
^
/safecode/llvm/include/llvm/Support/AlignOf.h:79:10: note: expanded from macro 'LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT'
    char alignas(x) aligned; \
         ^
/safecode/llvm/include/llvm/Support/AlignOf.h:93:1: error: 'alignas' attribute cannot be applied to types
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(8);
^
/safecode/llvm/include/llvm/Support/AlignOf.h:79:10: note: expanded from macro 'LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT'
    char alignas(x) aligned; \
```
