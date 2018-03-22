# Build tools and environment

AS = clang
CC = clang

CFLAGS = -g
LDFLAGS =

# Some configuration variables for the generated tests

TEST_SUFFIX = test
TEST_DIR = .

HARNESS_C = harness.c

C_HAYAI_DIR = ../C-Hayai/src
C_HAYAI_LDFLAGS = -I $(C_HAYAI_DIR) -lm
C_HAYAI_CFLAGS = $(C_HAYAI_DIR)/libchayai.a

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

# Projects not to build by default

PROJECTS_UNSTABLE = orangeduck-tgc

# Global commands

all: $(PROJECTS:=_$(TEST_SUFFIX))

clean: $(PROJECTS:=_clean)
	rm -f .*.o

test: $(addprefix exec_, $(PROJECTS:=_$(TEST_SUFFIX)))

# Default version of various commands

%_clean:
	rm -f "$(TEST_DIR)/$(patsubst %_clean,%_$(TEST_SUFFIX),$@)"

exec_%_$(TEST_SUFFIX): %_$(TEST_SUFFIX)
	./$<


########## https://github.com/100/Cranium

100-Cranium_$(TEST_SUFFIX): 100-Cranium 100-Cranium_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) 100-Cranium_test.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/100-Cranium_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm -lbenchmark -lpthread

########## https://github.com/adamdunkels/ubasic

adamdunkels-ubasic_$(TEST_SUFFIX): adamdunkels-ubasic adamdunkels-ubasic_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) adamdunkels-ubasic_test.c adamdunkels-ubasic/ubasic.c adamdunkels-ubasic/tokenizer.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/adamdunkels-ubasic_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/andikleen/snappy-c

andikleen-snappy-c_$(TEST_SUFFIX): andikleen-snappy-c andikleen-snappy-c_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) andikleen-snappy-c_test.c andikleen-snappy-c/snappy.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/andikleen-snappy-c_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/antirez/otree

antirez-otree_$(TEST_SUFFIX): antirez-otree antirez-otree_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) antirez-otree_test.c antirez-otree/btree.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/antirez-otree_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/antirez/rax

antirez-rax_$(TEST_SUFFIX):antirez-rax antirez-rax_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) antirez-rax_test.c antirez-rax/rax.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/antirez-rax_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/armon/libart

armon-libart_$(TEST_SUFFIX): armon-libart armon-libart_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) armon-libart_test.c armon-libart/src/art.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/armon-libart_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/bashrc/libdeep

bashrc-libdeep_$(TEST_SUFFIX): bashrc-libdeep bashrc-libdeep_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) bashrc-libdeep_test.c bashrc-libdeep/src/*.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/bashrc-libdeep_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm

########## https://github.com/camgunz/cmp

camgunz-cmp_$(TEST_SUFFIX): camgunz-cmp camgunz-cmp_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) camgunz-cmp_test.c camgunz-cmp/cmp.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/camgunz-cmp_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS)

########## https://github.com/chokkan/liblbfgs

chokkan-liblbfgs_$(TEST_SUFFIX): chokkan-liblbfgs chokkan-liblbfgs_test.c $(C_HAYAI_DIR)
	$(CC) $(CFLAGS) chokkan-liblbfgs_test.c chokkan-liblbfgs/lib/lbfgs.c $(C_HAYAI_CFLAGS) \
	 -o "$(TEST_DIR)/chokkan-liblbfgs_$(TEST_SUFFIX)" $(LDFLAGS) $(C_HAYAI_LDFLAGS) -lm -I chokkan-liblbfgs/include/

########## https://github.com/chrismoos/hash-ring

chrismoos-hash-ring_$(TEST_SUFFIX): chrismoos-hash-ring chrismoos-hash-ring_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) chrismoos-hash-ring_test.c chrismoos-hash-ring/hash_ring.c chrismoos-hash-ring/md5.c chrismoos-hash-ring/sha1.c -o "$(TEST_DIR)/chrismoos-hash-ring_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/cloudwu/cstring

cloudwu-cstring_$(TEST_SUFFIX): cloudwu-cstring cloudwu-cstring_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) cloudwu-cstring_test.c cloudwu-cstring/cstring.c -o "$(TEST_DIR)/cloudwu-cstring_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/codeplea/genann

codeplea-genann_$(TEST_SUFFIX): codeplea-genann codeplea-genann_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) codeplea-genann_test.c codeplea-genann/genann.c -o "$(TEST_DIR)/codeplea-genann_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/codeplea/tinyexpr

codeplea-tinyexpr_$(TEST_SUFFIX): codeplea-tinyexpr codeplea-tinyexpr_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) codeplea-tinyexpr_test.c codeplea-tinyexpr/tinyexpr.c -o "$(TEST_DIR)/codeplea-tinyexpr_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/ctz/cifra

ctz-cifra_$(TEST_SUFFIX): ctz-cifra ctz-cifra_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) ctz-cifra_test.c ctz-cifra/src/blockwise.c ctz-cifra/src/chash.c ctz-cifra/src/sha3.c -o "$(TEST_DIR)/ctz-cifra_$(TEST_SUFFIX)" $(LDFLAGS) -I ctz-cifra/src/ext

########## https://github.com/dbalmain/ferret

dbalmain-ferret_$(TEST_SUFFIX): dbalmain-ferret dbalmain-ferret_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) dbalmain-ferret_test.c dbalmain-ferret/c/src/*c dbalmain-ferret/c/lib/bzlib/*.c dbalmain-ferret/c/lib/libstemmer_c/libstemmer/libstemmer.c dbalmain-ferret/c/lib/libstemmer_c/src_c/*.c dbalmain-ferret/c/lib/libstemmer_c/runtime/*.c -o "$(TEST_DIR)/dbalmain-ferret_$(TEST_SUFFIX)" $(LDFLAGS) -lm -lpthread -I dbalmain-ferret/c/include

########## https://github.com/dcjones/hat-trie

dcjones-hat-trie_$(TEST_SUFFIX): dcjones-hat-trie dcjones-hat-trie_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) dcjones-hat-trie_test.c dcjones-hat-trie/src/*.c -o "$(TEST_DIR)/dcjones-hat-trie_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/dhess/c-ringbuf

dhess-c-ringbuf_$(TEST_SUFFIX): dhess-c-ringbuf dhess-c-ringbuf_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) dhess-c-ringbuf_test.c dhess-c-ringbuf/ringbuf.c -o "$(TEST_DIR)/dhess-c-ringbuf_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/fabianishere/brainfuck

fabianishere-brainfuck_$(TEST_SUFFIX): fabianishere-brainfuck fabianishere-brainfuck_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) fabianishere-brainfuck_test.c fabianishere-brainfuck/src/brainfuck.c -I fabianishere-brainfuck/include/ -o "$(TEST_DIR)/fabianishere-brainfuck_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/faragon/libsrt

faragon-libsrt_$(TEST_SUFFIX): faragon-libsrt faragon-libsrt_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) faragon-libsrt_test.c faragon-libsrt/src/*.c  faragon-libsrt/src/saux/*.c -o "$(TEST_DIR)/faragon-libsrt_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/ferreiradaselva/mathc

ferreiradaselva-mathc_$(TEST_SUFFIX): ferreiradaselva-mathc ferreiradaselva-mathc_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) ferreiradaselva-mathc_test.c ferreiradaselva-mathc/mathc.c -o "$(TEST_DIR)/ferreiradaselva-mathc_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/flame/blis

FLAME-BLIS_INC = flame-blis flame-blis/config/generic $(sort $(dir $(wildcard flame-blis/frame/*/))) $(sort $(dir $(wildcard flame-blis/frame/*/*/))) $(sort $(dir $(wildcard flame-blis/frame/*/*/*/)))
FLAME-BLIS_INC_PARAMS = $(foreach d, $(FLAME-BLIS_INC), -I$d)

flame-blis_$(TEST_SUFFIX): flame-blis flame-blis_test.c $(HARNESS_C)
	#(cd flame-blis && ./configure generic)
	(cd flame-blis && make)
	$(CC) $(CFLAGS) $(HARNESS_C) flame-blis_test.c -o "$(TEST_DIR)/flame-blis_$(TEST_SUFFIX)" $(LDFLAGS) ${FLAME-BLIS_INC_PARAMS} -lm -lpthread ./flame-blis/lib/generic/libblis.a

########## https://github.com/imneme/pcg-c-basic

imneme-pcg-c-basic_$(TEST_SUFFIX): imneme-pcg-c-basic imneme-pcg-c-basic_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) imneme-pcg-c-basic_test.c imneme-pcg-c-basic/pcg_basic.c -o "$(TEST_DIR)/imneme-pcg-c-basic_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/jtsiomb/kdtree

jtsiomb-kdtree_$(TEST_SUFFIX): jtsiomb-kdtree jtsiomb-kdtree_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) jtsiomb-kdtree_test.c jtsiomb-kdtree/kdtree.c -o "$(TEST_DIR)/jtsiomb-kdtree_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/josephg/librope

josephg-librope_$(TEST_SUFFIX): josephg-librope josephg-librope_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) josephg-librope_test.c josephg-librope/rope.c -o "$(TEST_DIR)/josephg-librope_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/Kazade/kazmath

Kazade-kazmath_$(TEST_SUFFIX): Kazade-kazmath Kazade-kazmath_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) Kazade-kazmath_test.c Kazade-kazmath/kazmath/*.c -o "$(TEST_DIR)/Kazade-kazmath_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/kbranigan/cJSON

kbranigan-cJSON_$(TEST_SUFFIX): kbranigan-cJSON kbranigan-cJSON_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) kbranigan-cJSON_test.c kbranigan-cJSON/cJSON.c -o "$(TEST_DIR)/kbranigan-cJSON_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/khovratovich/Argon2

khovratovich-Argon2_$(TEST_SUFFIX): khovratovich-Argon2 khovratovich-Argon2_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) khovratovich-Argon2_test.c khovratovich-Argon2/Source/C99/Argon2/argon2-core.c \
	 khovratovich-Argon2/Source/C99/Argon2/argon2.c \
	 khovratovich-Argon2/Source/C99/Argon2/argon2-ref-core.c \
	 khovratovich-Argon2/Source/C99/Argon2/kat.c \
	 khovratovich-Argon2/Source/C99/Blake2/blake2b-ref.c \
	 -o "$(TEST_DIR)/khovratovich-Argon2_$(TEST_SUFFIX)" $(LDFLAGS) -I khovratovich-Argon2/Source/C99/Common -lpthread

########## https://github.com/kuba--/zip

kuba---zip_$(TEST_SUFFIX): kuba---zip kuba---zip_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) kuba---zip_test.c kuba---zip/src/zip.c -o "$(TEST_DIR)/kuba---zip_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/kylophone/spectrophoto

kylophone-spectrophoto_$(TEST_SUFFIX): kylophone-spectrophoto kylophone-spectrophoto_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) kylophone-spectrophoto_test.c -o "$(TEST_DIR)/kylophone-spectrophoto_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/lacker/ikalman

lacker-ikalman_$(TEST_SUFFIX): lacker-ikalman lacker-ikalman_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) lacker-ikalman_test.c lacker-ikalman/gps.c lacker-ikalman/kalman.c lacker-ikalman/matrix.c -o "$(TEST_DIR)/lacker-ikalman_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/lemire/simdcomp

lemire-simdcomp_$(TEST_SUFFIX): lemire-simdcomp lemire-simdcomp_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) lemire-simdcomp_test.c lemire-simdcomp/src/*.c -o "$(TEST_DIR)/lemire-simdcomp_$(TEST_SUFFIX)" $(LDFLAGS) -I lemire-simdcomp/include

########## https://github.com/liteserver/binn

liteserver-binn_$(TEST_SUFFIX): liteserver-binn liteserver-binn_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) liteserver-binn_test.c liteserver-binn/src/binn.c -o "$(TEST_DIR)/liteserver-binn_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/lucasb-eyer/heatmap

lucasb-eyer-heatmap_$(TEST_SUFFIX): lucasb-eyer-heatmap lucasb-eyer-heatmap_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) lucasb-eyer-heatmap_test.c lucasb-eyer-heatmap/heatmap.c -o "$(TEST_DIR)/lucasb-eyer-heatmap_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/ludocode/mpack

ludocode-mpack_$(TEST_SUFFIX): ludocode-mpack ludocode-mpack_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) ludocode-mpack_test.c ludocode-mpack/src/mpack/*.c -o "$(TEST_DIR)/ludocode-mpack_$(TEST_SUFFIX)" $(LDFLAGS) -I ludocode-mpack/src

########## https://github.com/mbrossard/threadpool

mbrossard-threadpool_$(TEST_SUFFIX): mbrossard-threadpool mbrossard-threadpool_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) mbrossard-threadpool_test.c mbrossard-threadpool/src/threadpool.c -o "$(TEST_DIR)/mbrossard-threadpool_$(TEST_SUFFIX)" $(LDFLAGS) -lpthread

########## https://github.com/mongodb/libbson

mongodb-libbson_$(TEST_SUFFIX): mongodb-libbson mongodb-libbson_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) mongodb-libbson_test.c mongodb-libbson/src/bson/*.c mongodb-libbson/src/jsonsl/*.c -o "$(TEST_DIR)/mongodb-libbson_$(TEST_SUFFIX)" $(LDFLAGS) -I mongodb-libbson/src -I mongodb-libbson/src/bson -I includes/mongodb-libbson -D BSON_COMPILATION -lpthread

########## https://github.com/mrubyc/mrubyc

mrubyc-mrubyc_$(TEST_SUFFIX): mrubyc-mrubyc mrubyc-mrubyc_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) mrubyc-mrubyc_test.c mrubyc-mrubyc/src/*.c mrubyc-mrubyc/src/hal_posix/hal.c -o "$(TEST_DIR)/mrubyc-mrubyc_$(TEST_SUFFIX)" $(LDFLAGS) -I includes/mrubyc-mrubyc

########## https://github.com/msteinbeck/tinyspline

msteinbeck-tinyspline_$(TEST_SUFFIX): msteinbeck-tinyspline msteinbeck-tinyspline_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) msteinbeck-tinyspline_test.c msteinbeck-tinyspline/library/tinyspline.c -o "$(TEST_DIR)/msteinbeck-tinyspline_$(TEST_SUFFIX)" $(LDFLAGS) -lm -I msteinbeck-tinyspline/library

########## https://github.com/ndevilla/iniparser

ndevilla-iniparser_$(TEST_SUFFIX): ndevilla-iniparser ndevilla-iniparser_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) ndevilla-iniparser_test.c ndevilla-iniparser/src/*.c -o "$(TEST_DIR)/ndevilla-iniparser_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/orangeduck/tgc

orangeduck-tgc_$(TEST_SUFFIX): orangeduck-tgc orangeduck-tgc_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) orangeduck-tgc_test.c orangeduck-tgc/tgc.c -o "$(TEST_DIR)/orangeduck-tgc_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/paladin-t/my_basic

paladin-t-my_basic_$(TEST_SUFFIX): paladin-t-my_basic paladin-t-my_basic_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) paladin-t-my_basic_test.c paladin-t-my_basic/core/my_basic.c -o "$(TEST_DIR)/paladin-t-my_basic_$(TEST_SUFFIX)" $(LDFLAGS) -lm

########## https://github.com/prideout/heman

prideout-heman_$(TEST_SUFFIX): prideout-heman prideout-heman_test.c $(HARNESS_C)
	# -O2 because we need an always-inline pass in clang in order to compile
	$(CC) $(CFLAGS) -O2 $(HARNESS_C) prideout-heman_test.c prideout-heman/src/*.c prideout-heman/kazmath/*.c -o "$(TEST_DIR)/prideout-heman_$(TEST_SUFFIX)" $(LDFLAGS) -lm -I prideout-heman -I prideout-heman/include

########## https://github.com/quartzjer/js0n

quartzjer-js0n_$(TEST_SUFFIX): quartzjer-js0n quartzjer-js0n_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) quartzjer-js0n_test.c quartzjer-js0n/src/*c -o "$(TEST_DIR)/quartzjer-js0n_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/richgel999/miniz

richgel999-miniz_$(TEST_SUFFIX): richgel999-miniz richgel999-miniz_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) richgel999-miniz_test.c richgel999-miniz/*.c -o "$(TEST_DIR)/richgel999-miniz_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/RoaringBitmap/CRoaring

RoaringBitmap-CRoaring_$(TEST_SUFFIX): RoaringBitmap-CRoaring RoaringBitmap-CRoaring_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) RoaringBitmap-CRoaring_test.c RoaringBitmap-CRoaring/src/*.c RoaringBitmap-CRoaring/src/containers/*.c -o "$(TEST_DIR)/RoaringBitmap-CRoaring_$(TEST_SUFFIX)" $(LDFLAGS) -I RoaringBitmap-CRoaring/include/

########## https://github.com/rxi/vec

rxi-vec_$(TEST_SUFFIX): rxi-vec rxi-vec_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) rxi-vec_test.c rxi-vec/src/vec.c -o "$(TEST_DIR)/rxi-vec_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/skeeto/branchless-utf8

skeeto-branchless-utf8_$(TEST_SUFFIX): skeeto-branchless-utf8 skeeto-branchless-utf8_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) skeeto-branchless-utf8_test.c -o "$(TEST_DIR)/skeeto-branchless-utf8_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/snowballstem/snowball

snowballstem-snowball_$(TEST_SUFFIX): snowballstem-snowball snowballstem-snowball_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) snowballstem-snowball_test.c snowballstem-snowball/libstemmer.o -o "$(TEST_DIR)/snowballstem-snowball_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/swenson/sort

swenson-sort_$(TEST_SUFFIX): swenson-sort swenson-sort_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) swenson-sort_test.c -o "$(TEST_DIR)/swenson-sort_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/Themaister/libfmsynth

Themaister-libfmsynth_$(TEST_SUFFIX): Themaister-libfmsynth Themaister-libfmsynth_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) Themaister-libfmsynth_test.c Themaister-libfmsynth/src/fmsynth.c -o "$(TEST_DIR)/Themaister-libfmsynth_$(TEST_SUFFIX)" $(LDFLAGS) -lm -I Themaister-libfmsynth/include

########## https://github.com/tkengo/highway

tkengo-highway_$(TEST_SUFFIX): tkengo-highway tkengo-highway_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) tkengo-highway_test.c tkengo-highway/vendor/onigmo/*.c tkengo-highway/vendor/onigmo/*/*.c -o "$(TEST_DIR)/tkengo-highway_$(TEST_SUFFIX)" $(LDFLAGS) -I tkengo-highway/vendor/onigmo/

########## https://github.com/troydhanson/tpl

troydhanson-tpl_$(TEST_SUFFIX): troydhanson-tpl troydhanson-tpl_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) troydhanson-tpl_test.c troydhanson-tpl/src/tpl.c -o "$(TEST_DIR)/troydhanson-tpl_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/veorq/SipHash

veorq-SipHash_$(TEST_SUFFIX): veorq-SipHash veorq-SipHash_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) veorq-SipHash_test.c -o "$(TEST_DIR)/veorq-SipHash_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/vmg/crustache

vmg-crustache_$(TEST_SUFFIX): vmg-crustache vmg-crustache_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) vmg-crustache_test.c vmg-crustache/src/*.c -o "$(TEST_DIR)/vmg-crustache_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/wbhart/bsdnt

wbhart-bsdnt_$(TEST_SUFFIX): wbhart-bsdnt wbhart-bsdnt_test.c $(HARNESS_C)
	(cd ./wbhart-bsdnt && ./configure)
	(cd ./wbhart-bsdnt &&  make AS=$(AS) CC=$(CC) CFLAGS='$(CFLAGS) -fPIC')
	$(CC) $(CFLAGS) $(HARNESS_C) wbhart-bsdnt_test.c -o "$(TEST_DIR)/wbhart-bsdnt_$(TEST_SUFFIX)" $(LDFLAGS) -lm ./wbhart-bsdnt/libbsdnt.a

wbhart-bsdnt_clean:
ifneq ("$(wildcard ./wbhart-bsdnt/Makefile)","")
	(cd ./wbhart-bsdnt && make distclean && rm -f Makefile)
endif
	rm -f "$(TEST_DIR)/wbhart-bsdnt_$(TEST_SUFFIX)"

########## https://github.com/websnarf/bstrlib

websnarf-bstrlib_$(TEST_SUFFIX): websnarf-bstrlib websnarf-bstrlib_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) websnarf-bstrlib_test.c websnarf-bstrlib/bstraux.c websnarf-bstrlib/bstrlib.c -o "$(TEST_DIR)/websnarf-bstrlib_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/wolkykim/qlibc

wolkykim-qlibc_$(TEST_SUFFIX): wolkykim-qlibc wolkykim-qlibc_test.c $(HARNESS_C)
	(cd ./wolkykim-qlibc && ./configure)
	(cd ./wolkykim-qlibc &&  make)
	$(CC) $(CFLAGS) $(HARNESS_C) wolkykim-qlibc_test.c -o "$(TEST_DIR)/wolkykim-qlibc_$(TEST_SUFFIX)" $(LDFLAGS) -lpthread wolkykim-qlibc/lib/libqlibc.a

########## https://github.com/xamarin/flex

xamarin-flex_$(TEST_SUFFIX): xamarin-flex xamarin-flex_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) xamarin-flex_test.c xamarin-flex/flex.c -o "$(TEST_DIR)/xamarin-flex_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/yinqiwen/geohash-int

yinqiwen-geohash-int_$(TEST_SUFFIX): yinqiwen-geohash-int yinqiwen-geohash-int_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) yinqiwen-geohash-int_test.c yinqiwen-geohash-int/geohash.c -o "$(TEST_DIR)/yinqiwen-geohash-int_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/yosefk/checkedthreads

yosefk-checkedthreads_$(TEST_SUFFIX): yosefk-checkedthreads yosefk-checkedthreads_test.c $(HARNESS_C)
	(cd ./yosefk-checkedthreads && python2 ./configure)
	$(CC) $(CFLAGS) $(HARNESS_C) yosefk-checkedthreads_test.c yosefk-checkedthreads/src/*.c -o "$(TEST_DIR)/yosefk-checkedthreads_$(TEST_SUFFIX)" $(LDFLAGS) -lpthread -I yosefk-checkedthreads/include

########## https://github.com/zedshaw/liblcthw

zedshaw-liblcthw_$(TEST_SUFFIX): zedshaw-liblcthw zedshaw-liblcthw_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) zedshaw-liblcthw_test.c zedshaw-liblcthw/src/lcthw/bstree.c zedshaw-liblcthw/src/lcthw/bstrlib.c -o "$(TEST_DIR)/zedshaw-liblcthw_$(TEST_SUFFIX)" $(LDFLAGS) -I zedshaw-liblcthw/src

########## https://github.com/zhemao/libds

zhemao-libds_$(TEST_SUFFIX): zhemao-libds zhemao-libds_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) zhemao-libds_test.c zhemao-libds/vector.c -o "$(TEST_DIR)/zhemao-libds_$(TEST_SUFFIX)" $(LDFLAGS)

########## https://github.com/zserge/partcl

zserge-partcl_$(TEST_SUFFIX): zserge-partcl zserge-partcl_test.c $(HARNESS_C)
	$(CC) $(CFLAGS) $(HARNESS_C) zserge-partcl_test.c -o "$(TEST_DIR)/zserge-partcl_$(TEST_SUFFIX)" $(LDFLAGS)