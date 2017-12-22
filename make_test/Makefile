# Build tools and environment

AS=clang
CC=clang

# Some configuration variables for the generated tests

TEST_SUFFIX=test
TEST_DIR=.

HARNESS_C=harness.c

# Projects to build

PROJECTS = andikleen-snappy-c antirez-otree bashrc-libdeep camgunz-cmp chrismoos-hash-ring codeplea-genann ctz-cifra dcjones-hat-trie fabianishere-brainfuck faragon-libsrt ferreiradaselva-mathc jtsiomb-kdtree josephg-librope khovratovich-Argon2 lucasb-eyer-heatmap RoaringBitmap-CRoaring snowballstem-snowball swenson-sort Themaister-libfmsynth troydhanson-tpl veorq-SipHash wbhart-bsdnt websnarf-bstrlib zhemao-libds

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

########## https://github.com/chrismoos/hash-ring

chrismoos-hash-ring_$(TEST_SUFFIX): chrismoos-hash-ring chrismoos-hash-ring_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) chrismoos-hash-ring_test.c chrismoos-hash-ring/hash_ring.c chrismoos-hash-ring/md5.c chrismoos-hash-ring/sha1.c -o "$(TEST_DIR)/chrismoos-hash-ring_$(TEST_SUFFIX)"

########## https://github.com/codeplea/genann

codeplea-genann_$(TEST_SUFFIX): codeplea-genann codeplea-genann_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) codeplea-genann_test.c codeplea-genann/genann.c -o "$(TEST_DIR)/codeplea-genann_$(TEST_SUFFIX)" -lm

########## https://github.com/ctz/cifra

ctz-cifra_$(TEST_SUFFIX): ctz-cifra ctz-cifra_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) ctz-cifra_test.c ctz-cifra/src/blockwise.c ctz-cifra/src/chash.c ctz-cifra/src/sha3.c -o "$(TEST_DIR)/ctz-cifra_$(TEST_SUFFIX)" -I ctz-cifra/src/ext

########## https://github.com/dcjones/hat-trie

dcjones-hat-trie_$(TEST_SUFFIX): dcjones-hat-trie dcjones-hat-trie_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) dcjones-hat-trie_test.c dcjones-hat-trie/src/*.c -o "$(TEST_DIR)/dcjones-hat-trie_$(TEST_SUFFIX)"

########## https://github.com/fabianishere/brainfuck

fabianishere-brainfuck_$(TEST_SUFFIX): fabianishere-brainfuck fabianishere-brainfuck_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) fabianishere-brainfuck_test.c fabianishere-brainfuck/src/brainfuck.c -I fabianishere-brainfuck/include/ -o "$(TEST_DIR)/fabianishere-brainfuck_$(TEST_SUFFIX)"

########## https://github.com/faragon/libsrt

faragon-libsrt_$(TEST_SUFFIX): faragon-libsrt faragon-libsrt_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) faragon-libsrt_test.c faragon-libsrt/src/*.c  faragon-libsrt/src/saux/*.c -o "$(TEST_DIR)/faragon-libsrt_$(TEST_SUFFIX)"

########## https://github.com/ferreiradaselva/mathc

ferreiradaselva-mathc_$(TEST_SUFFIX): ferreiradaselva-mathc ferreiradaselva-mathc_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) ferreiradaselva-mathc_test.c ferreiradaselva-mathc/mathc.c -o "$(TEST_DIR)/ferreiradaselva-mathc_$(TEST_SUFFIX)" -lm

########## https://github.com/jtsiomb/kdtree

jtsiomb-kdtree_$(TEST_SUFFIX): jtsiomb-kdtree jtsiomb-kdtree_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) jtsiomb-kdtree_test.c jtsiomb-kdtree/kdtree.c -o "$(TEST_DIR)/jtsiomb-kdtree_$(TEST_SUFFIX)"

########## https://github.com/khovratovich/Argon2

khovratovich-Argon2_$(TEST_SUFFIX): khovratovich-Argon2 khovratovich-Argon2_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) khovratovich-Argon2_test.c khovratovich-Argon2/Source/C99/Argon2/argon2-core.c \
	 khovratovich-Argon2/Source/C99/Argon2/argon2.c \
	 khovratovich-Argon2/Source/C99/Argon2/argon2-ref-core.c \
	 khovratovich-Argon2/Source/C99/Argon2/kat.c \
	 khovratovich-Argon2/Source/C99/Blake2/blake2b-ref.c \
	 -o "$(TEST_DIR)/khovratovich-Argon2_$(TEST_SUFFIX)" -I khovratovich-Argon2/Source/C99/Common -pthread

########## https://github.com/josephg/librope

josephg-librope_$(TEST_SUFFIX): josephg-librope josephg-librope_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) josephg-librope_test.c josephg-librope/rope.c -o "$(TEST_DIR)/josephg-librope_$(TEST_SUFFIX)"

########## https://github.com/lucasb-eyer/heatmap

lucasb-eyer-heatmap_$(TEST_SUFFIX): lucasb-eyer-heatmap lucasb-eyer-heatmap_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) lucasb-eyer-heatmap_test.c lucasb-eyer-heatmap/heatmap.c -o "$(TEST_DIR)/lucasb-eyer-heatmap_$(TEST_SUFFIX)" -lm

########## https://github.com/RoaringBitmap/CRoaring

RoaringBitmap-CRoaring_$(TEST_SUFFIX): RoaringBitmap-CRoaring RoaringBitmap-CRoaring_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) RoaringBitmap-CRoaring_test.c RoaringBitmap-CRoaring/src/*.c RoaringBitmap-CRoaring/src/containers/*.c -o "$(TEST_DIR)/RoaringBitmap-CRoaring_$(TEST_SUFFIX)" -I RoaringBitmap-CRoaring/include/

########## https://github.com/snowballstem/snowball

snowballstem-snowball_$(TEST_SUFFIX): snowballstem-snowball snowballstem-snowball_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) snowballstem-snowball_test.c snowballstem-snowball/libstemmer.o -o "$(TEST_DIR)/snowballstem-snowball_$(TEST_SUFFIX)"

########## https://github.com/swenson/sort

swenson-sort_$(TEST_SUFFIX): swenson-sort swenson-sort_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) swenson-sort_test.c -o "$(TEST_DIR)/swenson-sort_$(TEST_SUFFIX)"

########## https://github.com/Themaister/libfmsynth

Themaister-libfmsynth_$(TEST_SUFFIX): Themaister-libfmsynth Themaister-libfmsynth_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) Themaister-libfmsynth_test.c Themaister-libfmsynth/src/fmsynth.c -o "$(TEST_DIR)/Themaister-libfmsynth_$(TEST_SUFFIX)" -lm -I Themaister-libfmsynth/include

########## https://github.com/troydhanson/tpl

troydhanson-tpl_$(TEST_SUFFIX): troydhanson-tpl troydhanson-tpl_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) troydhanson-tpl_test.c troydhanson-tpl/src/tpl.c -o "$(TEST_DIR)/troydhanson-tpl_$(TEST_SUFFIX)"

########## https://github.com/veorq/SipHash

veorq-SipHash_$(TEST_SUFFIX): veorq-SipHash veorq-SipHash_test.c $(HARNESS_C)
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

########## https://github.com/websnarf/bstrlib

websnarf-bstrlib_$(TEST_SUFFIX): websnarf-bstrlib websnarf-bstrlib_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) websnarf-bstrlib_test.c websnarf-bstrlib/bstraux.c websnarf-bstrlib/bstrlib.c -o "$(TEST_DIR)/websnarf-bstrlib_$(TEST_SUFFIX)"

########## https://github.com/zhemao/libds

zhemao-libds_$(TEST_SUFFIX): zhemao-libds zhemao-libds_test.c $(HARNESS_C)
	$(CC) $(HARNESS_C) zhemao-libds_test.c zhemao-libds/vector.c -o "$(TEST_DIR)/zhemao-libds_$(TEST_SUFFIX)"
