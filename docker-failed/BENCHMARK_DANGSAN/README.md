# dangsan

## Build Error

The installation script builds its own ld, make,... and one of the configure steps causes the (custom) linker to
not recognize one of the arguments. This argument is likely inserted by some package on our ubuntu 18.04 system

```
configure:3405: checking for C compiler version
configure:3414: gcc --version >&5
gcc (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

configure:3425: $? = 0
configure:3414: gcc -v >&5
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/7/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 7.3.0-27ubuntu1~18.04' --with-bugurl=file:///usr/share/doc/gcc-7/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++ --prefix=/usr --with-gcc-major-version-only --program-suffix=-7 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-libmpx --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 7.3.0 (Ubuntu 7.3.0-27ubuntu1~18.04) 
configure:3425: $? = 0
configure:3414: gcc -V >&5
gcc: error: unrecognized command line option '-V'
gcc: fatal error: no input files
compilation terminated.
configure:3425: $? = 1
configure:3414: gcc -qversion >&5
gcc: error: unrecognized command line option '-qversion'; did you mean '--version'?
gcc: fatal error: no input files
compilation terminated.
configure:3425: $? = 1
configure:3445: checking whether the C compiler works
configure:3467: gcc    conftest.c  >&5
/dangsan/autosetup.dir/install/common/bin/ld: --push-state: unknown option
/dangsan/autosetup.dir/install/common/bin/ld: use the --help option for usage information
collect2: error: ld returned 1 exit status
configure:3471: $? = 1
configure:3509: result: no
configure: failed program was:
| /* confdefs.h */
| #define PACKAGE_NAME "GNU make"
| #define PACKAGE_TARNAME "make"
| #define PACKAGE_VERSION "4.2"
| #define PACKAGE_STRING "GNU make 4.2"
| #define PACKAGE_BUGREPORT "bug-make@gnu.org"
| #define PACKAGE_URL "http://www.gnu.org/software/make/"
| #define PACKAGE "make"
| #define VERSION "4.2"
| /* end confdefs.h.  */
| 
| int
| main ()
| {
| 
|   ;
|   return 0;
| }
configure:3514: error: in `/dangsan/autosetup.dir/packages/obj/make-4.2':
configure:3516: error: C compiler cannot create executables
See `config.log' for more details
```
