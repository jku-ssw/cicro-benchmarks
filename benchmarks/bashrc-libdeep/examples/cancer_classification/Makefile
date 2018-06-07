all:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -o cancerdetect cancerdetect.c -ldeep -lm -fopenmp -ffast-math

debug:
	gcc -Wall -ansi -pedantic -mindirect-branch=thunk -g -o cancerdetect cancerdetect.c -ldeep -lm -fopenmp -ffast-math

clean:
	rm -f *.o cancerdetect export* *.plist
