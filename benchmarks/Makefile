# Build tools and environment

AS = clang
CC = clang

CFLAGS =
LDFLAGS =

# Some configuration variables for the generated tests

TEST_SUFFIX = test
TEST_DIR = .

C_HAYAI_DIR = ../C-Hayai
C_HAYAI_LDFLAGS = -I $(C_HAYAI_DIR)/src -lm
C_HAYAI_LIB = $(C_HAYAI_DIR)/build/src/libchayai.a
C_HAYAI_CFLAGS = $(C_HAYAI_LIB)

USE_PAPI_VALUE = ON
ifdef PAPI
ifeq '$(PAPI)' '0'
	USE_PAPI_VALUE = OFF
endif
endif

ifeq '$(USE_PAPI_VALUE)' 'ON'
	C_HAYAI_LDFLAGS += -lpapi
	C_HAYAI_CFLAGS += -DUSE_PAPI
endif

# Projects to build

PROJECTS = 100-Cranium adamdunkels-ubasic andikleen-snappy-c antirez-otree antirez-rax armon-libart bashrc-libdeep\
		   camgunz-cmp chokkan-liblbfgs chrismoos-hash-ring cloudwu-cstring codeplea-genann codeplea-tinyexpr ctz-cifra\
		   dbalmain-ferret dcjones-hat-trie dhess-c-ringbuf fabianishere-brainfuck faragon-libsrt ferreiradaselva-mathc flame-blis\
		   imneme-pcg-c-basic jtsiomb-kdtree josephg-librope Kazade-kazmath kbranigan-cJSON khovratovich-Argon2 kuba---zip\
		   kylophone-spectrophoto lacker-ikalman lemire-simdcomp liteserver-binn lucasb-eyer-heatmap ludocode-mpack mbrossard-threadpool\
		   mongodb-libbson mrubyc-mrubyc msteinbeck-tinyspline ndevilla-iniparser paladin-t-my_basic prideout-heman\
		   quartzjer-js0n richgel999-miniz RoaringBitmap-CRoaring rxi-vec skeeto-branchless-utf8 snowballstem-snowball\
		   swenson-sort Themaister-libfmsynth tkengo-highway troydhanson-tpl veorq-SipHash vmg-crustache wbhart-bsdnt websnarf-bstrlib\
		   wolkykim-qlibc xamarin-flex yinqiwen-geohash-int yosefk-checkedthreads zedshaw-liblcthw zhemao-libds zserge-partcl

UNSTABLE_PROJECTS = orangeduck-tgc # runtime can vary by factor 100!

# Global commands

all: $(PROJECTS:=_$(TEST_SUFFIX))

clean: $(PROJECTS:=_clean)
	rm -f *.o .*.o
	rm -f *.bc .*.bc
	rm -rf $(C_HAYAI_DIR)/build

test: $(addprefix exec_, $(PROJECTS:=_$(TEST_SUFFIX)))

# Default version of various commands

%_clean:
	rm -f "$(TEST_DIR)/$(patsubst %_clean,%_$(TEST_SUFFIX),$@)"

exec_%_$(TEST_SUFFIX): %_$(TEST_SUFFIX)
	./$<

# Build Benchmark Runner

$(C_HAYAI_LIB): $(C_HAYAI_DIR)/src/*
	mkdir -p $(C_HAYAI_DIR)/build
	(cd $(C_HAYAI_DIR)/build && cmake ../ -DCMAKE_C_COMPILER='$(CC)' -DCMAKE_C_FLAGS='$(CFLAGS)' -DCMAKE_STATIC_LINKER_FLAGS='$(LDFLAGS)' -DUSE_PAPI=$(USE_PAPI_VALUE))
	$(MAKE) -C $(C_HAYAI_DIR)/build -j4 AS='$(AS)' CC='$(CC)' CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)'

#.PHONY: $(C_HAYAI_LIB)

########## https://github.com/100/Cranium

100-Cranium_$(TEST_SUFFIX): 100-Cranium 100-Cranium_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) 100-Cranium_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/100-Cranium_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm -lpthread

########## https://github.com/adamdunkels/ubasic

adamdunkels-ubasic_$(TEST_SUFFIX): adamdunkels-ubasic adamdunkels-ubasic_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) adamdunkels-ubasic_test.c adamdunkels-ubasic/ubasic.c adamdunkels-ubasic/tokenizer.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/adamdunkels-ubasic_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/andikleen/snappy-c

andikleen-snappy-c_$(TEST_SUFFIX): andikleen-snappy-c andikleen-snappy-c_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) andikleen-snappy-c_test.c andikleen-snappy-c/snappy.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/andikleen-snappy-c_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/antirez/otree

antirez-otree_$(TEST_SUFFIX): antirez-otree antirez-otree_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) antirez-otree_test.c antirez-otree/btree.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/antirez-otree_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/antirez/rax

antirez-rax_$(TEST_SUFFIX):antirez-rax antirez-rax_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) antirez-rax_test.c antirez-rax/rax.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/antirez-rax_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/armon/libart

armon-libart_$(TEST_SUFFIX): armon-libart armon-libart_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) armon-libart_test.c armon-libart/src/art.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/armon-libart_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/bashrc/libdeep

bashrc-libdeep_$(TEST_SUFFIX): bashrc-libdeep bashrc-libdeep_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) bashrc-libdeep_test.c bashrc-libdeep/src/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/bashrc-libdeep_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/camgunz/cmp

camgunz-cmp_$(TEST_SUFFIX): camgunz-cmp camgunz-cmp_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) camgunz-cmp_test.c camgunz-cmp/cmp.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/camgunz-cmp_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/chokkan/liblbfgs

chokkan-liblbfgs_$(TEST_SUFFIX): chokkan-liblbfgs chokkan-liblbfgs_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) chokkan-liblbfgs_test.c chokkan-liblbfgs/lib/lbfgs.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/chokkan-liblbfgs_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm -I chokkan-liblbfgs/include/

########## https://github.com/chrismoos/hash-ring

chrismoos-hash-ring_$(TEST_SUFFIX): chrismoos-hash-ring chrismoos-hash-ring_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) chrismoos-hash-ring_test.c chrismoos-hash-ring/hash_ring.c chrismoos-hash-ring/md5.c chrismoos-hash-ring/sha1.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/chrismoos-hash-ring_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/cloudwu/cstring

cloudwu-cstring_$(TEST_SUFFIX): cloudwu-cstring cloudwu-cstring_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) cloudwu-cstring_test.c cloudwu-cstring/cstring.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/cloudwu-cstring_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/codeplea/genann

codeplea-genann_$(TEST_SUFFIX): codeplea-genann codeplea-genann_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) codeplea-genann_test.c codeplea-genann/genann.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/codeplea-genann_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/codeplea/tinyexpr

codeplea-tinyexpr_$(TEST_SUFFIX): codeplea-tinyexpr codeplea-tinyexpr_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) codeplea-tinyexpr_test.c codeplea-tinyexpr/tinyexpr.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/codeplea-tinyexpr_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/ctz/cifra

ctz-cifra_$(TEST_SUFFIX): ctz-cifra ctz-cifra_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) ctz-cifra_test.c ctz-cifra/src/blockwise.c ctz-cifra/src/chash.c ctz-cifra/src/sha3.c ctz-cifra/src/chacha20.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/ctz-cifra_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I ctz-cifra/src/ext

########## https://github.com/dbalmain/ferret

dbalmain-ferret_$(TEST_SUFFIX): dbalmain-ferret dbalmain-ferret_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) dbalmain-ferret_test.c dbalmain-ferret/c/src/*c dbalmain-ferret/c/lib/bzlib/*.c \
	 dbalmain-ferret/c/lib/libstemmer_c/libstemmer/libstemmer.c dbalmain-ferret/c/lib/libstemmer_c/src_c/*.c \
	 dbalmain-ferret/c/lib/libstemmer_c/runtime/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/dbalmain-ferret_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm -lpthread -I dbalmain-ferret/c/include -I dbalmain-ferret/c/lib/libstemmer_c/include

########## https://github.com/dcjones/hat-trie

dcjones-hat-trie_$(TEST_SUFFIX): dcjones-hat-trie dcjones-hat-trie_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) dcjones-hat-trie_test.c dcjones-hat-trie/src/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/dcjones-hat-trie_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/dhess/c-ringbuf

dhess-c-ringbuf_$(TEST_SUFFIX): dhess-c-ringbuf dhess-c-ringbuf_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) dhess-c-ringbuf_test.c dhess-c-ringbuf/ringbuf.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/dhess-c-ringbuf_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/fabianishere/brainfuck

fabianishere-brainfuck_$(TEST_SUFFIX): fabianishere-brainfuck fabianishere-brainfuck_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) fabianishere-brainfuck_test.c fabianishere-brainfuck/src/brainfuck.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/fabianishere-brainfuck_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I fabianishere-brainfuck/include/

########## https://github.com/faragon/libsrt

faragon-libsrt_$(TEST_SUFFIX): faragon-libsrt faragon-libsrt_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) faragon-libsrt_test.c faragon-libsrt/src/*.c  faragon-libsrt/src/saux/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/faragon-libsrt_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/ferreiradaselva/mathc

ferreiradaselva-mathc_$(TEST_SUFFIX): ferreiradaselva-mathc ferreiradaselva-mathc_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) ferreiradaselva-mathc_test.c ferreiradaselva-mathc/mathc.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/ferreiradaselva-mathc_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/flame/blis

FLAME-BLIS_INC = flame-blis flame-blis/config/generic $(sort $(dir $(wildcard flame-blis/frame/*/))) $(sort $(dir $(wildcard flame-blis/frame/*/*/))) $(sort $(dir $(wildcard flame-blis/frame/*/*/*/)))
FLAME-BLIS_INC_PARAMS = $(foreach d, $(FLAME-BLIS_INC), -I$d)

flame-blis_$(TEST_SUFFIX): flame-blis flame-blis_test.c $(C_HAYAI_LIB)
	(cd flame-blis && ./configure generic)
	(cd ./flame-blis && $(MAKE) -j4 AS='$(AS)' CC='$(CC)' CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)')
	$(CC) $(CFLAGS) flame-blis_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/flame-blis_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) ${FLAME-BLIS_INC_PARAMS} -lm -lpthread ./flame-blis/lib/generic/libblis.a

flame-blis_clean:
	(cd ./flame-blis && $(MAKE) clean cleanmk; exit 0)
	rm -f "$(TEST_DIR)/flame-blis_$(TEST_SUFFIX)"

########## https://github.com/imneme/pcg-c-basic

imneme-pcg-c-basic_$(TEST_SUFFIX): imneme-pcg-c-basic imneme-pcg-c-basic_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) imneme-pcg-c-basic_test.c imneme-pcg-c-basic/pcg_basic.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/imneme-pcg-c-basic_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/jtsiomb/kdtree

jtsiomb-kdtree_$(TEST_SUFFIX): jtsiomb-kdtree jtsiomb-kdtree_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) jtsiomb-kdtree_test.c jtsiomb-kdtree/kdtree.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/jtsiomb-kdtree_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/josephg/librope

josephg-librope_$(TEST_SUFFIX): josephg-librope josephg-librope_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) josephg-librope_test.c josephg-librope/rope.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/josephg-librope_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/Kazade/kazmath

Kazade-kazmath_$(TEST_SUFFIX): Kazade-kazmath Kazade-kazmath_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) Kazade-kazmath_test.c Kazade-kazmath/kazmath/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/Kazade-kazmath_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/kbranigan/cJSON

kbranigan-cJSON_$(TEST_SUFFIX): kbranigan-cJSON kbranigan-cJSON_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) kbranigan-cJSON_test.c kbranigan-cJSON/cJSON.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/kbranigan-cJSON_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/khovratovich/Argon2

khovratovich-Argon2_$(TEST_SUFFIX): khovratovich-Argon2 khovratovich-Argon2_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) khovratovich-Argon2_test.c khovratovich-Argon2/Source/C99/Argon2/argon2-core.c \
	 khovratovich-Argon2/Source/C99/Argon2/argon2.c \
	 khovratovich-Argon2/Source/C99/Argon2/argon2-ref-core.c \
	 khovratovich-Argon2/Source/C99/Argon2/kat.c \
	 khovratovich-Argon2/Source/C99/Blake2/blake2b-ref.c \
	 $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/khovratovich-Argon2_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I khovratovich-Argon2/Source/C99/Common -lpthread

########## https://github.com/kuba--/zip

kuba---zip_$(TEST_SUFFIX): kuba---zip kuba---zip_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) kuba---zip_test.c kuba---zip/src/zip.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/kuba---zip_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/kylophone/spectrophoto

kylophone-spectrophoto_$(TEST_SUFFIX): kylophone-spectrophoto kylophone-spectrophoto_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) kylophone-spectrophoto_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/kylophone-spectrophoto_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/lacker/ikalman

lacker-ikalman_$(TEST_SUFFIX): lacker-ikalman lacker-ikalman_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) lacker-ikalman_test.c lacker-ikalman/gps.c lacker-ikalman/kalman.c lacker-ikalman/matrix.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/lacker-ikalman_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/lemire/simdcomp

lemire-simdcomp_$(TEST_SUFFIX): lemire-simdcomp lemire-simdcomp_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) lemire-simdcomp_test.c lemire-simdcomp/src/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/lemire-simdcomp_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I lemire-simdcomp/include

########## https://github.com/liteserver/binn

liteserver-binn_$(TEST_SUFFIX): liteserver-binn liteserver-binn_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) liteserver-binn_test.c liteserver-binn/src/binn.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/liteserver-binn_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/lucasb-eyer/heatmap

lucasb-eyer-heatmap_$(TEST_SUFFIX): lucasb-eyer-heatmap lucasb-eyer-heatmap_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) lucasb-eyer-heatmap_test.c lucasb-eyer-heatmap/heatmap.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/lucasb-eyer-heatmap_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/ludocode/mpack

ludocode-mpack_$(TEST_SUFFIX): ludocode-mpack ludocode-mpack_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) ludocode-mpack_test.c ludocode-mpack/src/mpack/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/ludocode-mpack_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I ludocode-mpack/src

########## https://github.com/mbrossard/threadpool

mbrossard-threadpool_$(TEST_SUFFIX): mbrossard-threadpool mbrossard-threadpool_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) mbrossard-threadpool_test.c mbrossard-threadpool/src/threadpool.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/mbrossard-threadpool_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lpthread

########## https://github.com/mongodb/libbson

mongodb-libbson_$(TEST_SUFFIX): mongodb-libbson mongodb-libbson_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) mongodb-libbson_test.c mongodb-libbson/src/bson/*.c mongodb-libbson/src/jsonsl/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/mongodb-libbson_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I mongodb-libbson/src -I mongodb-libbson/src/bson -I includes/mongodb-libbson -D BSON_COMPILATION -lpthread

########## https://github.com/mrubyc/mrubyc

mrubyc-mrubyc_$(TEST_SUFFIX): mrubyc-mrubyc mrubyc-mrubyc_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) mrubyc-mrubyc_test.c mrubyc-mrubyc/src/*.c mrubyc-mrubyc/src/hal_posix/hal.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/mrubyc-mrubyc_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I includes/mrubyc-mrubyc

########## https://github.com/msteinbeck/tinyspline

msteinbeck-tinyspline_$(TEST_SUFFIX): msteinbeck-tinyspline msteinbeck-tinyspline_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) msteinbeck-tinyspline_test.c msteinbeck-tinyspline/library/tinyspline.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/msteinbeck-tinyspline_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm -I msteinbeck-tinyspline/library

########## https://github.com/ndevilla/iniparser

ndevilla-iniparser_$(TEST_SUFFIX): ndevilla-iniparser ndevilla-iniparser_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) ndevilla-iniparser_test.c ndevilla-iniparser/src/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/ndevilla-iniparser_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/orangeduck/tgc

orangeduck-tgc_$(TEST_SUFFIX): orangeduck-tgc orangeduck-tgc_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) orangeduck-tgc_test.c orangeduck-tgc/tgc.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/orangeduck-tgc_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/paladin-t/my_basic

paladin-t-my_basic_$(TEST_SUFFIX): paladin-t-my_basic paladin-t-my_basic_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) paladin-t-my_basic_test.c paladin-t-my_basic/core/my_basic.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/paladin-t-my_basic_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/prideout/heman

prideout-heman_$(TEST_SUFFIX): prideout-heman prideout-heman_test.c $(C_HAYAI_LIB)
	# -O2 because we need an always-inline pass in clang in order to compile
	$(CC) $(CFLAGS) -O2 prideout-heman_test.c prideout-heman/src/*.c prideout-heman/kazmath/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/prideout-heman_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm -I prideout-heman -I prideout-heman/include

########## https://github.com/quartzjer/js0n

quartzjer-js0n_$(TEST_SUFFIX): quartzjer-js0n quartzjer-js0n_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) quartzjer-js0n_test.c quartzjer-js0n/src/*c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/quartzjer-js0n_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/richgel999/miniz

richgel999-miniz_$(TEST_SUFFIX): richgel999-miniz richgel999-miniz_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) richgel999-miniz_test.c richgel999-miniz/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/richgel999-miniz_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/RoaringBitmap/CRoaring

RoaringBitmap-CRoaring_$(TEST_SUFFIX): RoaringBitmap-CRoaring RoaringBitmap-CRoaring_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) RoaringBitmap-CRoaring_test.c RoaringBitmap-CRoaring/src/*.c RoaringBitmap-CRoaring/src/containers/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/RoaringBitmap-CRoaring_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I RoaringBitmap-CRoaring/include/

########## https://github.com/rxi/vec

rxi-vec_$(TEST_SUFFIX): rxi-vec rxi-vec_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) rxi-vec_test.c rxi-vec/src/vec.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/rxi-vec_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/skeeto/branchless-utf8

skeeto-branchless-utf8_$(TEST_SUFFIX): skeeto-branchless-utf8 skeeto-branchless-utf8_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) skeeto-branchless-utf8_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/skeeto-branchless-utf8_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/snowballstem/snowball

snowballstem-snowball_$(TEST_SUFFIX): snowballstem-snowball snowballstem-snowball_test.c $(C_HAYAI_LIB)
	(cd ./snowballstem-snowball && $(MAKE) -j4 AS='$(AS)' CC='$(CC)' CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)')
	$(CC) $(CFLAGS) snowballstem-snowball_test.c snowballstem-snowball/libstemmer.o $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/snowballstem-snowball_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

snowballstem-snowball_clean:
	(cd ./snowballstem-snowball && $(MAKE) clean)
	rm -f "$(TEST_DIR)/snowballstem-snowball_$(TEST_SUFFIX)"

########## https://github.com/swenson/sort

swenson-sort_$(TEST_SUFFIX): swenson-sort swenson-sort_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) swenson-sort_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/swenson-sort_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/Themaister/libfmsynth

Themaister-libfmsynth_$(TEST_SUFFIX): Themaister-libfmsynth Themaister-libfmsynth_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) Themaister-libfmsynth_test.c Themaister-libfmsynth/src/fmsynth.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/Themaister-libfmsynth_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm -I Themaister-libfmsynth/include

########## https://github.com/tkengo/highway

tkengo-highway_$(TEST_SUFFIX): tkengo-highway tkengo-highway_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) tkengo-highway_test.c tkengo-highway/vendor/onigmo/*.c tkengo-highway/vendor/onigmo/*/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/tkengo-highway_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I tkengo-highway/vendor/onigmo/

########## https://github.com/troydhanson/tpl

troydhanson-tpl_$(TEST_SUFFIX): troydhanson-tpl troydhanson-tpl_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) troydhanson-tpl_test.c troydhanson-tpl/src/tpl.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/troydhanson-tpl_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/veorq/SipHash

veorq-SipHash_$(TEST_SUFFIX): veorq-SipHash veorq-SipHash_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) veorq-SipHash_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/veorq-SipHash_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/vmg/crustache

vmg-crustache_$(TEST_SUFFIX): vmg-crustache vmg-crustache_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) vmg-crustache_test.c vmg-crustache/src/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/vmg-crustache_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/wbhart/bsdnt

wbhart-bsdnt_$(TEST_SUFFIX): wbhart-bsdnt wbhart-bsdnt_test.c $(C_HAYAI_LIB)
	(cd ./wbhart-bsdnt && ./configure AS='$(AS)' CC='$(CC)')
	(cd ./wbhart-bsdnt &&  $(MAKE) -j4 AS='$(AS)' CC='$(CC)' CFLAGS='$(CFLAGS) -fPIC' LDFLAGS='$(LDFLAGS)')
	$(CC) $(CFLAGS) wbhart-bsdnt_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/wbhart-bsdnt_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm ./wbhart-bsdnt/libbsdnt.a

wbhart-bsdnt_clean:
ifneq ("$(wildcard ./wbhart-bsdnt/Makefile)","")
	(cd ./wbhart-bsdnt && $(MAKE) distclean && rm -f Makefile)
endif
	rm -f "$(TEST_DIR)/wbhart-bsdnt_$(TEST_SUFFIX)"

########## https://github.com/websnarf/bstrlib

websnarf-bstrlib_$(TEST_SUFFIX): websnarf-bstrlib websnarf-bstrlib_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) websnarf-bstrlib_test.c websnarf-bstrlib/bstraux.c websnarf-bstrlib/bstrlib.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/websnarf-bstrlib_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/wolkykim/qlibc

wolkykim-qlibc_$(TEST_SUFFIX): wolkykim-qlibc wolkykim-qlibc_test.c $(C_HAYAI_LIB)
	(cd ./wolkykim-qlibc && ./configure  CC='$(CC)' CFLAGS='$(CFLAGS) -fPIC' LDFLAGS='$(LDFLAGS)')
	(cd ./wolkykim-qlibc &&  $(MAKE) -j4 AS='$(AS)' CC='$(CC)' CFLAGS='$(CFLAGS) -fPIC' LDFLAGS='$(LDFLAGS)')
	$(CC) $(CFLAGS) wolkykim-qlibc_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/wolkykim-qlibc_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lpthread wolkykim-qlibc/lib/libqlibc.a

wolkykim-qlibc_clean:
ifneq ("$(wildcard ./wolkykim-qlibc/Makefile)","")
	(cd ./wolkykim-qlibc && $(MAKE) distclean && rm -f Makefile)
endif
	rm -f "$(TEST_DIR)/wolkykim-qlibc_$(TEST_SUFFIX)"

########## https://github.com/xamarin/flex

xamarin-flex_$(TEST_SUFFIX): xamarin-flex xamarin-flex_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) xamarin-flex_test.c xamarin-flex/flex.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/xamarin-flex_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/yinqiwen/geohash-int

yinqiwen-geohash-int_$(TEST_SUFFIX): yinqiwen-geohash-int yinqiwen-geohash-int_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) yinqiwen-geohash-int_test.c yinqiwen-geohash-int/geohash.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/yinqiwen-geohash-int_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/yosefk/checkedthreads

yosefk-checkedthreads_$(TEST_SUFFIX): yosefk-checkedthreads yosefk-checkedthreads_test.c $(C_HAYAI_LIB)
	(cd ./yosefk-checkedthreads && python2 ./configure)
	$(CC) $(CFLAGS) yosefk-checkedthreads_test.c yosefk-checkedthreads/src/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/yosefk-checkedthreads_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lpthread -I yosefk-checkedthreads/include

########## https://github.com/zedshaw/liblcthw

zedshaw-liblcthw_$(TEST_SUFFIX): zedshaw-liblcthw zedshaw-liblcthw_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) zedshaw-liblcthw_test.c zedshaw-liblcthw/src/lcthw/bstree.c zedshaw-liblcthw/src/lcthw/bstrlib.c \
	 zedshaw-liblcthw/src/lcthw/hashmap.c zedshaw-liblcthw/src/lcthw/darray.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/zedshaw-liblcthw_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -I zedshaw-liblcthw/src

########## https://github.com/zhemao/libds

zhemao-libds_$(TEST_SUFFIX): zhemao-libds zhemao-libds_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) zhemao-libds_test.c zhemao-libds/vector.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/zhemao-libds_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/zserge/partcl

zserge-partcl_$(TEST_SUFFIX): zserge-partcl zserge-partcl_test.c $(C_HAYAI_LIB)
	$(CC) $(CFLAGS) zserge-partcl_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/zserge-partcl_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)
