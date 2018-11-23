# stroke

## build problems

Stroke requires GCC4.9

Pin requires maximum GCC <4.5 because otherwise ABI is no longer compatible

thus, there is no way to compile the application in its original state due to dependency conflicts

```
make[2]: Leaving directory '/stoke/src/ext/x64asm'
Z3 was successfully built.
Z3Py scripts can already be executed in the 'build' directory.
Z3Py scripts stored in arbitrary directories can be also executed if 'build' directory is added to the PYTHONPATH environment variable.
Use the following command to install Z3 at prefix /usr.
    sudo make install
make[2]: Leaving directory '/stoke/src/ext/z3/build'
In file included from ../../../source/include/pin/pin.H:41:0,
                 from testcase.cpp:33:
../../../source/include/pin/compiler_version_check2.H:102:2: error: #error The C++ ABI of your compiler does not match the ABI of the pin kit.
 #error The C++ ABI of your compiler does not match the ABI of the pin kit.
  ^
../../../source/include/pin/compiler_version_check2.H:117:2: error: #error This kit requires gcc 3.4 or later
 #error This kit requires gcc 3.4 or later
  ^
Makefile:77: recipe for target 'obj-intel64/testcase.o' failed
make[2]: *** [obj-intel64/testcase.o] Error 1
make[2]: Leaving directory '/stoke/src/ext/pin-2.13-62732-gcc.4.4.7-linux/source/tools/stoke'
Makefile:327: recipe for target 'pintool' failed
make[1]: *** [pintool] Error 2
make[1]: Leaving directory '/stoke'
Makefile:256: recipe for target 'release' failed
make: *** [release] Error 2
```
