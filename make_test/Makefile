# Build tools and environment

AS=clang
CC=clang

# Some configuration variables for the generated tests

TEST_SUFFIX=test
TEST_DIR=.

HARNESS_C=harness.c

# Projects to build

PROJECTS = antirez-otree wbhart-bsdnt

# Global commands

all: $(PROJECTS:=_$(TEST_SUFFIX))

clean: $(PROJECTS:=_clean)
	rm .*.o

test: $(addprefix exec_, $(PROJECTS:=_$(TEST_SUFFIX)))

# Default version of various commands

%_clean:
	rm -f "$(TEST_DIR)/$(patsubst %_clean,%_$(TEST_SUFFIX),$@)"

exec_%_$(TEST_SUFFIX): %_$(TEST_SUFFIX)
	./$<


########## https://github.com/antirez/otree

antirez-otree_$(TEST_SUFFIX): antirez-otree antirez-otree_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) antirez-otree_test.c antirez-otree/btree.c -o "$(TEST_DIR)/antirez-otree_$(TEST_SUFFIX)"

########## https://github.com/wbhart/bsdnt

wbhart-bsdnt_$(TEST_SUFFIX): wbhart-bsdnt wbhart-bsdnt_test.c $(HARNESS_C)
	(cd ./wbhart-bsdnt; ./configure)
	(cd ./wbhart-bsdnt; make AS=$(AS) CC=$(CC))
	$(CC) $(HARNESS_C) wbhart-bsdnt_test.c -o "$(TEST_DIR)/wbhart-bsdnt_$(TEST_SUFFIX)" -lm ./wbhart-bsdnt/libbsdnt.a

wbhart-bsdnt_clean:
ifneq ("$(wildcard ./wbhart-bsdnt/Makefile)","")
	(cd ./wbhart-bsdnt; make distclean; rm -f Makefile)
endif
	rm -f "$(TEST_DIR)/wbhart-bsdnt_$(TEST_SUFFIX)"
