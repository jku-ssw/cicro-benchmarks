
.PHONY: check-syntax

all:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -o concreteslump concreteslump.c -lm -fopenmp -ffast-math -I../../src/ ../../libdeep-1.00.so.0.0.1

check-syntax:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -o concreteslump concreteslump.c -lm -fopenmp -ffast-math -fsyntax-only -I../../src/ ../../libdeep-1.00.so.0.0.1

debug:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -g -o concreteslump concreteslump.c -lm -fopenmp -ffast-math -I../../src/ ../../libdeep-1.00.so.0.0.1

clean:
	rm -f *.o concreteslump *.plist export* *.png
