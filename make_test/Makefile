# Build tools and environment

AS=clang
CC=clang

# Some configuration variables for the generated tests

TEST_SUFFIX=test
TEST_DIR=.

# Projects to build

PROJECTS = wbhart-bsdnt

# Global commands

all: $(PROJECTS:=_$(TEST_SUFFIX))

clean: $(PROJECTS:=_clean)

test: $(addprefix exec_, $(PROJECTS:=_$(TEST_SUFFIX)))

# Default version of various commands

%_clean:
	rm -f "$(TEST_DIR)/$(patsubst %_clean,%_$(TEST_SUFFIX),$@)"

exec_%_$(TEST_SUFFIX): %_$(TEST_SUFFIX)
	./$<

########## https://github.com/wbhart/bsdnt

wbhart-bsdnt_$(TEST_SUFFIX):
	(cd ./wbhart-bsdnt; ./configure)
	(cd ./wbhart-bsdnt; make AS=$(AS) CC=$(CC))
	$(CC) wbhart-bsdnt_test.c -o "$(TEST_DIR)/wbhart-bsdnt_$(TEST_SUFFIX)"

wbhart-bsdnt_clean:
ifneq ("$(wildcard ./wbhart-bsdnt/Makefile)","")
	(cd ./wbhart-bsdnt; make distclean; rm -f Makefile)
endif
	rm -f "$(TEST_DIR)/wbhart-bsdnt_$(TEST_SUFFIX)"
