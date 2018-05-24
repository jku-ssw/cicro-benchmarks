APP=libdeep
VERSION=1.00
RELEASE=1
SONAME=${APP}.so.0
LIBNAME=${APP}-${VERSION}.so.0.0.${RELEASE}
ARCH_TYPE=`uname -m`
PREFIX?=/usr/local
LIBDIR=lib
ARCH_BUILD_DIR=${HOME}/abs/${APP}
CURR_DIR=$(shell pwd)
SELF_DIR=$(shell basename $(CURR_DIR))
SOURCEFILE?=backprop.c

DATE_FMT = %Y-%m-%d
ifdef SOURCE_DATE_EPOCH
	BUILD_DATE ?= $(shell date -u -d "@$(SOURCE_DATE_EPOCH)" "+$(DATE_FMT)"  2>/dev/null || date -u -r "$(SOURCE_DATE_EPOCH)" "+$(DATE_FMT)" 2>/dev/null || date -u "+$(DATE_FMT)")
else
	BUILD_DATE ?= $(shell date "+$(DATE_FMT)")
endif

ifeq ($(shell if [ -d /usr/lib64 ]; then echo "found"; fi;), "found")
LIBDIR = lib64
endif

all:
	rm -f src/flycheck*
	gcc -shared -Wl,-soname,${SONAME} -std=c99 -pedantic -mindirect-branch=thunk -fPIC -O3 -o ${LIBNAME} src/*.c -Isrc -lm -fopenmp -ffast-math
debug:
	rm -f src/flycheck*
	gcc -shared -Wl,-soname,${SONAME} -std=c99 -pedantic -mindirect-branch=thunk -fPIC -g -o ${LIBNAME} src/*.c -Isrc -lm -fopenmp -ffast-math
debugstack:
	rm -f src/flycheck*
	gcc -shared -Wl,-soname,${SONAME} -std=c99 -pedantic -mindirect-branch=thunk -fsanitize=address -fPIC -g -o ${LIBNAME} src/*.c -Isrc -lm -fopenmp -ffast-math
graph:
	rm -f src/flycheck*
	gcc -shared -Wl,-soname,${SONAME} -std=c99 -pedantic -mindirect-branch=thunk -fPIC -g -o ${LIBNAME} src/*.c -Isrc -lm -fopenmp -fdump-rtl-expand -ffast-math
	egypt ${SOURCEFILE}.*.expand | xdot -
	rm *.expand
source:
	tar -cvf ../${APP}_${VERSION}.orig.tar --exclude-vcs ../$(SELF_DIR)
	gzip -f9n ../${APP}_${VERSION}.orig.tar
arch:
	rm -f ${APP} *.xz *.sig
	@if [ ! -d ${ARCH_BUILD_DIR} ]; then\
		mkdir -p ${ARCH_BUILD_DIR};\
	fi
	rm -rf ${ARCH_BUILD_DIR}/*
	tar cvf ${ARCH_BUILD_DIR}/${APP}-${VERSION}.tar --exclude-vcs .
	gzip -f9n ${ARCH_BUILD_DIR}/${APP}-${VERSION}.tar
	cp PKGBUILD ${ARCH_BUILD_DIR}
	gpg -ba ${ARCH_BUILD_DIR}/${APP}-${VERSION}.tar.gz
	sed -i "s|arch=()|arch=('${ARCH_TYPE}')|g" ${ARCH_BUILD_DIR}/PKGBUILD
	cd ${ARCH_BUILD_DIR}; updpkgsums; makepkg -f -c -s; makepkg --printsrcinfo > .SRCINFO
	unxz ${ARCH_BUILD_DIR}/${APP}-${VERSION}-${RELEASE}-${ARCH_TYPE}.pkg.tar.xz
	tar vf ${ARCH_BUILD_DIR}/${APP}-${VERSION}-${RELEASE}-${ARCH_TYPE}.pkg.tar --delete .BUILDINFO
	xz ${ARCH_BUILD_DIR}/${APP}-${VERSION}-${RELEASE}-${ARCH_TYPE}.pkg.tar
	gpg -ba ${ARCH_BUILD_DIR}/${APP}-${VERSION}-${RELEASE}-${ARCH_TYPE}.pkg.tar.xz
install:
	mkdir -p ${DESTDIR}/usr
	mkdir -p ${DESTDIR}${PREFIX}/${LIBDIR}/${APP}
	mkdir -p ${DESTDIR}${PREFIX}/include/${APP}
	cp src/*.h ${DESTDIR}${PREFIX}/include/${APP}
	install -m 755 ${LIBNAME} ${DESTDIR}${PREFIX}/${LIBDIR}
	ln -sf ${DESTDIR}${PREFIX}/${LIBDIR}/${LIBNAME} ${DESTDIR}${PREFIX}/${LIBDIR}/${SONAME}
	ln -sf ${DESTDIR}${PREFIX}/${LIBDIR}/${LIBNAME} ${DESTDIR}/${LIBDIR}/${SONAME}
	ln -sf ${DESTDIR}${PREFIX}/${LIBDIR}/${LIBNAME} ${DESTDIR}${PREFIX}/${LIBDIR}/${APP}.so
	ldconfig
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man/man1
	install -m 644 man/${APP}.1.gz ${DESTDIR}${PREFIX}/share/man/man1
uninstall:
	rm -f ${PREFIX}/share/man/man1/${APP}.1.gz
	rm -f ${PREFIX}/${LIBDIR}/${LIBNAME}
	rm -f ${PREFIX}/${LIBDIR}/${APP}.so
	rm -f ${PREFIX}/${LIBDIR}/${SONAME}
	rm -f /${LIBDIR}/${SONAME}
	rm -rf ${PREFIX}/include/${APP}
	ldconfig
instlib:
	mkdir -p ${DESTDIR}/usr
	mkdir -p ${DESTDIR}${PREFIX}/${LIBDIR}/${APP}
	mkdir -p ${DESTDIR}${PREFIX}/include/${APP}
	cp src/*.h ${DESTDIR}${PREFIX}/include/${APP}
	install -m 755 ${LIBNAME} ${DESTDIR}${PREFIX}/${LIBDIR}
	mkdir -m 755 -p ${DESTDIR}${PREFIX}/share/man/man1
	install -m 644 man/${APP}.1.gz ${DESTDIR}${PREFIX}/share/man/man1
clean:
	rm -f ${LIBNAME} \#* \.#* gnuplot* *.png src/*.plist *.expand
	rm -f unittests/*.plist unittests/tests
