# rpmalloc

## Build error

```
Step 7/10 : RUN cd rpmalloc-1.3.1 && python3 configure.py && /ninja
 ---> Running in 05a73b11279e
[1/48] MKDIR lib/linux/debug
[2/48] MKDIR lib/linux/release
[3/48] MKDIR 'lib/linux/debug/b'\''x86_64'\'''
[4/48] MKDIR 'lib/linux/release/b'\''x86_64'\'''
[5/48] MKDIR bin/linux/debug
[6/48] MKDIR 'bin/linux/debug/b'\''x86_64'\'''
[7/48] CC rpmalloc/malloc.c
FAILED: build/ninja/linux/debug/b'x86_64'/rpmalloc-a8e50b6/malloc-53b207c.o 
clang -MMD -MT 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/malloc-53b207c.o' -MF 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/malloc-53b207c.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=c11  -DBUILD_DEBUG=1 -g -DENABLE_PRELOAD=1 -c rpmalloc/malloc.c -o 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/malloc-53b207c.o'
depfile has multiple output paths[8/48] CXX rpmalloc/new.cc

FAILED: build/ninja/linux/debug/b'x86_64'/rpmalloc-a8e50b6/new-440206f.o 
clang++ -MMD -MT 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/new-440206f.o' -MF 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/new-440206f.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=gnu++14  -DBUILD_DEBUG=1 -g -DENABLE_PRELOAD=1 -c rpmalloc/new.cc -o 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/new-440206f.o'
depfile has multiple output paths[9/48] CXX rpmalloc/new.cc

FAILED: build/ninja/linux/release/b'x86_64'/rpmalloc-a8e50b6/new-440206f.o 
clang++ -MMD -MT 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/new-440206f.o' -MF 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/new-440206f.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=gnu++14  -DBUILD_RELEASE=1 -O3 -g -funroll-loops -DENABLE_PRELOAD=1 -c rpmalloc/new.cc -o 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/new-440206f.o'
depfile has multiple output paths[10/48] CC rpmalloc/rpmalloc.c

FAILED: build/ninja/linux/debug/b'x86_64'/rpmalloc-7c2f09b/rpmalloc-65d008a.o 
clang -MMD -MT 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-7c2f09b/rpmalloc-65d008a.o' -MF 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-7c2f09b/rpmalloc-65d008a.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=c11  -DBUILD_DEBUG=1 -g  -c rpmalloc/rpmalloc.c -o 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-7c2f09b/rpmalloc-65d008a.o'
depfile has multiple output paths[11/48] CC rpmalloc/malloc.c

FAILED: build/ninja/linux/release/b'x86_64'/rpmalloc-a8e50b6/malloc-53b207c.o 
clang -MMD -MT 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/malloc-53b207c.o' -MF 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/malloc-53b207c.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=c11  -DBUILD_RELEASE=1 -O3 -g -funroll-loops -DENABLE_PRELOAD=1 -c rpmalloc/malloc.c -o 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/malloc-53b207c.o'
depfile has multiple output paths[12/48] CC rpmalloc/rpmalloc.c

FAILED: build/ninja/linux/debug/b'x86_64'/rpmalloc-a8e50b6/rpmalloc-65d008a.o 
clang -MMD -MT 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/rpmalloc-65d008a.o' -MF 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/rpmalloc-65d008a.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=c11  -DBUILD_DEBUG=1 -g -DENABLE_PRELOAD=1 -c rpmalloc/rpmalloc.c -o 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-a8e50b6/rpmalloc-65d008a.o'
depfile has multiple output paths[13/48] CC rpmalloc/rpmalloc.c

FAILED: build/ninja/linux/debug/b'x86_64'/rpmalloc-cccf0ca/rpmalloc-a2b50b2.o 
clang -MMD -MT 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-cccf0ca/rpmalloc-a2b50b2.o' -MF 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-cccf0ca/rpmalloc-a2b50b2.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=c11 -DBUILD_DYNAMIC_LINK=1 -fPIC -DBUILD_DEBUG=1 -g  -c rpmalloc/rpmalloc.c -o 'build/ninja/linux/debug/b'\''x86_64'\''/rpmalloc-cccf0ca/rpmalloc-a2b50b2.o'
depfile has multiple output paths[14/48] CC rpmalloc/rpmalloc.c

FAILED: build/ninja/linux/release/b'x86_64'/rpmalloc-a8e50b6/rpmalloc-65d008a.o 
clang -MMD -MT 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/rpmalloc-65d008a.o' -MF 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/rpmalloc-65d008a.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=c11  -DBUILD_RELEASE=1 -O3 -g -funroll-loops -DENABLE_PRELOAD=1 -c rpmalloc/rpmalloc.c -o 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-a8e50b6/rpmalloc-65d008a.o'
depfile has multiple output paths[15/48] CC rpmalloc/rpmalloc.c

FAILED: build/ninja/linux/release/b'x86_64'/rpmalloc-7c2f09b/rpmalloc-65d008a.o 
clang -MMD -MT 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-7c2f09b/rpmalloc-65d008a.o' -MF 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-7c2f09b/rpmalloc-65d008a.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=c11  -DBUILD_RELEASE=1 -O3 -g -funroll-loops  -c rpmalloc/rpmalloc.c -o 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-7c2f09b/rpmalloc-65d008a.o'
depfile has multiple output paths[16/48] CC rpmalloc/rpmalloc.c

FAILED: build/ninja/linux/release/b'x86_64'/rpmalloc-cccf0ca/rpmalloc-a2b50b2.o 
clang -MMD -MT 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-cccf0ca/rpmalloc-a2b50b2.o' -MF 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-cccf0ca/rpmalloc-a2b50b2.o'.d -I.  -DRPMALLOC_COMPILE=1 -funit-at-a-time -fstrict-aliasing -fno-math-errno -ffinite-math-only -funsafe-math-optimizations -fno-trapping-math -ffast-math -D_GNU_SOURCE=1 -W -Werror -pedantic -Wall -Weverything -Wno-padded -Wno-documentation-unknown-command -std=c11 -DBUILD_DYNAMIC_LINK=1 -fPIC -DBUILD_RELEASE=1 -O3 -g -funroll-loops  -c rpmalloc/rpmalloc.c -o 'build/ninja/linux/release/b'\''x86_64'\''/rpmalloc-cccf0ca/rpmalloc-a2b50b2.o'
depfile has multiple output paths
ninja: build stopped: subcommand failed.
The command '/bin/sh -c cd rpmalloc-1.3.1 && python3 configure.py && /ninja' returned a non-zero code: 1
Makefile:13: recipe for target 'build' failed
```

## Bug report

* https://github.com/rampantpixels/rpmalloc/issues/77
