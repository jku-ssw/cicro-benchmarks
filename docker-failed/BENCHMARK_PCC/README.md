# Portable C Compiler (PCC)

## Build errors

```
===> libpcc
make[1]: Entering directory '/pcc-libs-20170101/libpcc'
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c cmpdi2.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c divdi3.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c fixdfdi.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c fixsfdi.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c fixunsdfdi.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c fixunssfdi.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c floatdidf.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c floatdisf.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c floatunsdidf.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c moddi3.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c muldi3.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c negdi2.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c qdivrem.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c ucmpdi2.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c udivdi3.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c umoddi3.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c cxmuldiv.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c ashldi3.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c ashrdi3.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c lshrdi3.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c _alloca.c
pcc -O -I. -DTARGET_LITTLE_ENDIAN=1 -Dos_linux -Dmach_amd64 -isystem ../libpcc/include  -c unwind.c
/usr/include/x86_64-linux-gnu//bits/floatn.h, line 75: warning: unknown mode arg __TC__
/usr/include/x86_64-linux-gnu//bits/floatn.h, line 87: invalid function definition
/usr/include/x86_64-linux-gnu//bits/floatn.h, line 87: syntax error
/usr/include/x86_64-linux-gnu//bits/floatn.h, line 87: invalid function definition
/usr/include//stdlib.h, line 363: cannot recover from earlier errors: goodbye!
error: /usr/lib/x86_64-linux-gnu/ccom terminated with status 1
Makefile:52: recipe for target 'unwind.o' failed
make[1]: Leaving directory '/pcc-libs-20170101/libpcc'
```

## Bug reports

The issue tracker on http://pcc.ludd.ltu.se/jira is down.
