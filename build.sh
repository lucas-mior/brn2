#!/bin/sh

# shellcheck disable=SC2086

dir="$(realpath "$(dirname "$0")")"
testing () {
    for src in *.c; do
        [ "$src" = "$main" ] && continue
        printf "Testing $src...\n"

        flags="$(awk '/\/\/ flags:/ { $1=$2=""; print $0 }' "$src")"
        cmdline="$CC $CPPFLAGS $CFLAGS \
                 -D TESTING_THIS_FILE=1 $src -o /tmp/$src.exe $flags"
        set -x
        if $cmdline; then
            /tmp/$src.exe || gdb /tmp/$src.exe
        else
            printf "Failed to compile ${RED} $src ${RES}, is main() defined?\n"
            exit
        fi

        set +x 
    done
}

NFILES=500000
d="/tmp/brn2"

create_files() {
    rm -rf "$d"
    mkdir -p "$d"
    cd "$d" || exit
    seq -w $NFILES | sed 's/^/000/g' | xargs -P"$(nproc)" touch
    cd "$dir" || exit
}

benchmark() {
    create_files
    cd "$d" || exit
    # strace -f -c -o $dir/strace.txt $dir/brn2 -s -q -d . 2>&1
    time $dir/brn2 -s -q -d . 2>&1
    cd "$dir" || exit
}

callgrind() {
    create_files
    cd "$d" || exit
    valgrind --tool=callgrind \
        --collect-systime=msec \
        --dump-instr=yes \
        --callgrind-out-file=$dir/brn2_$1.out \
        $dir/brn2 -s -q -r .
    cd "$dir" || exit
    setsid -f kcachegrind "$dir/brn2_$1.out" > /dev/null 2>&1
}

target="${1:-build}"
PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

main="main.c"
program="brn2"

CFLAGS="$CFLAGS -std=c99 -D_DEFAULT_SOURCE "
CFLAGS="$CFLAGS -Wextra -Wall -Wno-unused-macros "
LDFLAGS="$LDFLAGS -lm "

CC=${CC:-cc}
if [ $CC = "clang" ]; then
    CFLAGS="$CFLAGS -Weverything "
    CFLAGS="$CFLAGS -Wno-unsafe-buffer-usage -Wno-format-nonliteral "
    CFLAGS="$CFLAGS -Wno-disabled-macro-expansion "
fi

case "$target" in
    "debug")
        CFLAGS="$CFLAGS -g -fsanitize=undefined"
        CPPFLAGS="$CPPFLAGS -DBRN2_DEBUG=1" ;;
    "benchmark")
        CFLAGS="$CFLAGS    -O2 -flto -march=native -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK" ;;
    "callgrind") 
        CFLAGS="$CFLAGS -g -O2 -flto -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK" ;;
    "test") 
        CFLAGS="$CFLAGS -g -O2 -flto -march=native -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS " ;;
    "*") 
        CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS -DBRN2_DEBUG=0" ;;
esac

case "$target" in
    "uninstall")
        set -x
        rm -f ${DESTDIR}${PREFIX}/bin/${program}
        rm -f ${DESTDIR}${PREFIX}/man/man1/${program}.1
        ;;
    "test")
        testing
        ;;
    "install")
        [ ! -f $program ] && $0 build
        set -x
        install -Dm755 ${program} ${DESTDIR}${PREFIX}/bin/${program}
        install -Dm644 ${program}.1 ${DESTDIR}${PREFIX}/man/man1/${program}.1
        ;;
    "build"|"debug"|"benchmark"|"callgrind")
        set -x
        ctags --kinds-C=+l -- *.h *.c 2> /dev/null || true
        vtags.sed tags > .tags.vim 2> /dev/null || true
        $CC $CPPFLAGS $CFLAGS -o ${program} "$main" $LDFLAGS
        ;;
    *)
        echo "usage: $0 [ uninstall / test / install / build / debug ]"
        ;;
esac

case "$target" in
    "benchmark") benchmark "$2" ;;
    "callgrind") callgrind "$2" ;;
esac
