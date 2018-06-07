APP=tests

.PHONY: check-syntax

all:
	gcc -Wall -std=c99 -pedantic -mindirect-branch=thunk -g -o $(APP) *.c ../src/*.c -I../src -lm -fopenmp -ffast-math
check-syntax:
	gcc -Wall -std=c99 -pedantic -mindirect-branch=thunk -fsanitize=address -g -o $(APP) *.c ../src/*.c -I../src -lm -fsyntax-only
debug:
	gcc -Wall -std=c99 -pedantic -mindirect-branch=thunk -g -o $(APP) *.c ../src/*.c -I../src -lm -fopenmp -ffast-math
debugstack:
	gcc -Wall -std=c99 -pedantic -mindirect-branch=thunk -fsanitize=address -g -o $(APP) *.c ../src/*.c -I../src -lm -fopenmp -ffast-math
clean:
	rm -f ${APP} *.plist
