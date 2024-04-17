#!/bin/sh


testing () {
    for src in *.c; do
        [ "$src" = "main.c" ] && continue
        printf "Testing $src...\n"

        flags="$(awk '/flags:/ { $1=$2=""; print $0 }' "$src")"
        gcc -D TESTING_THIS_FILE=1 $src -o $src.exe $flags \
            || printf "${RED}Failed to compile $src, is main() defined? ${RES}\n"

        ./$src.exe
    done
    rm *.exe
}

PREFIX="${PREFIX:-/usr/local}"

SRC=$(ls *.c)

CFLAGS+=" -std=c99 -D_DEFAULT_SOURCE "
CFLAGS+=" -Wextra -Wall "
LDFLAGS+=" -lm "

CC=${CC:-clang}
if [ $CC = "clang" ]; then
    CFLAGS+=" -Weverything -Wno-unsafe-buffer-usage -Wno-format-nonliteral "
fi

if [ "$1" == "release" ]; then
    CFLAGS+=" -O2 -flto "
else
    CFLAGS+=" -g -fsanitize=undefined "
fi


if [ "$1" == "uninstall" ]; then
    set -x
	rm -f ${DESTDIR}${PREFIX}/bin/brn2
	rm -f ${DESTDIR}${PREFIX}/man/man1/brn2.1
elif [ "$1" == "test" ]; then
    testing
else
    set -x
	ctags --kinds-C=+l *.h *.c
	vtags.sed tags > .tags.vim
    $CC $CFLAGS -o brn2 main.c $LDFLAGS

    install -Dm755 brn2 ${DESTDIR}${PREFIX}/bin/brn2
    install -Dm644 brn2.1 ${DESTDIR}${PREFIX}/man/man1/brn2.1
fi
