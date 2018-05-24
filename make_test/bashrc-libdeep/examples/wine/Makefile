.PHONY: check-syntax

all:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -o winequality winequality.c -ldeep -lm -fopenmp -ffast-math

check-syntax:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -o winequality winequality.c -ldeep -lm -fopenmp -fsyntax-only -ffast-math

debug:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -g -o winequality winequality.c -ldeep -lm -fopenmp -ffast-math

clean:
	rm -f *.o winequality
