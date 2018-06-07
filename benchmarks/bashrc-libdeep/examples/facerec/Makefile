
.PHONY: check-syntax

all:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -o facerec facerec.c -ldeep -lm -fopenmp -ffast-math

check-syntax:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -o facerec facerec.c -ldeep -lm -fopenmp -fsyntax-only -ffast-math

debug:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -g -o facerec facerec.c -ldeep -lm -fopenmp -ffast-math

clean:
	rm -f *.o facerec *.png *.plist
