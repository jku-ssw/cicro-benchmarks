# Build tools and environment

AS=clang
CC=clang

# Some configuration variables for the generated tests

TEST_SUFFIX=test
TEST_DIR=.

HARNESS_C=harness.c

# Projects to build

PROJECTS = andikleen-snappy-c antirez-otree bashrc-libdeep camgunz-cmp dcjones-hat-trie fabianishere-brainfuck josephg-librope lucasb-eyer-heatmap snowballstem-snowball swenson-sort troydhanson-tpl veorq-SipHash wbhart-bsdnt zhemao-libds

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


########## https://github.com/andikleen/snappy-c

andikleen-snappy-c_$(TEST_SUFFIX): andikleen-snappy-c andikleen-snappy-c_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) andikleen-snappy-c_test.c andikleen-snappy-c/snappy.c -o "$(TEST_DIR)/andikleen-snappy-c_$(TEST_SUFFIX)"

########## https://github.com/antirez/otree

antirez-otree_$(TEST_SUFFIX): antirez-otree antirez-otree_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) antirez-otree_test.c antirez-otree/btree.c -o "$(TEST_DIR)/antirez-otree_$(TEST_SUFFIX)"

########## https://github.com/bashrc/libdeep

bashrc-libdeep_$(TEST_SUFFIX): bashrc-libdeep bashrc-libdeep_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) bashrc-libdeep_test.c bashrc-libdeep/src/*.c -o "$(TEST_DIR)/bashrc-libdeep_$(TEST_SUFFIX)" -lm

########## https://github.com/camgunz/cmp

camgunz-cmp_$(TEST_SUFFIX): camgunz-cmp camgunz-cmp_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) camgunz-cmp_test.c camgunz-cmp/cmp.c -o "$(TEST_DIR)/camgunz-cmp_$(TEST_SUFFIX)"

########## https://github.com/josephg/librope

josephg-librope_$(TEST_SUFFIX): josephg-librope josephg-librope_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) josephg-librope_test.c josephg-librope/rope.c -o "$(TEST_DIR)/josephg-librope_$(TEST_SUFFIX)"

########## https://github.com/dcjones/hat-trie

dcjones-hat-trie_$(TEST_SUFFIX): dcjones-hat-trie dcjones-hat-trie_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) dcjones-hat-trie_test.c dcjones-hat-trie/src/*.c -o "$(TEST_DIR)/dcjones-hat-trie_$(TEST_SUFFIX)"

########## https://github.com/lucasb-eyer/heatmap

lucasb-eyer-heatmap_$(TEST_SUFFIX): lucasb-eyer-heatmap lucasb-eyer-heatmap_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) lucasb-eyer-heatmap_test.c lucasb-eyer-heatmap/heatmap.c -o "$(TEST_DIR)/lucasb-eyer-heatmap_$(TEST_SUFFIX)" -lm

########## https://github.com/dcjones/hat-trie

fabianishere-brainfuck_$(TEST_SUFFIX): fabianishere-brainfuck fabianishere-brainfuck_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) fabianishere-brainfuck_test.c fabianishere-brainfuck/src/brainfuck.c -I fabianishere-brainfuck/include/ -o "$(TEST_DIR)/fabianishere-brainfuck_$(TEST_SUFFIX)"

########## https://github.com/snowballstem/snowball

snowballstem-snowball_$(TEST_SUFFIX): snowballstem-snowball snowballstem-snowball_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) snowballstem-snowball_test.c snowballstem-snowball/libstemmer.o -o "$(TEST_DIR)/snowballstem-snowball_$(TEST_SUFFIX)"

########## https://github.com/swenson/sort

swenson-sort_$(TEST_SUFFIX): swenson-sort swenson-sort_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) swenson-sort_test.c -o "$(TEST_DIR)/swenson-sort_$(TEST_SUFFIX)"

########## https://github.com/troydhanson/tpl

troydhanson-tpl_$(TEST_SUFFIX): troydhanson-tpl troydhanson-tpl_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) troydhanson-tpl_test.c troydhanson-tpl/src/tpl.c -o "$(TEST_DIR)/troydhanson-tpl_$(TEST_SUFFIX)"

########## https://github.com/veorq/SipHash

veorq-SipHash_$(TEST_SUFFIX):veorq-SipHash veorq-SipHash_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) veorq-SipHash_test.c -o "$(TEST_DIR)/veorq-SipHash_$(TEST_SUFFIX)"

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

########## https://github.com/zhemao/libds

zhemao-libds_$(TEST_SUFFIX): zhemao-libds zhemao-libds_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) zhemao-libds_test.c zhemao-libds/vector.c -o "$(TEST_DIR)/zhemao-libds_$(TEST_SUFFIX)"
