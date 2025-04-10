#!/bin/sh

# shellcheck disable=SC2086

targets='
test
build
debug
benchmark
valgrind
check
'

dir="$(realpath "$(dirname "$0")")"

NFILES=500000
d="/tmp/brn2"

create_files() {
    rm -rf "$d"
    mkdir -p "$d"
    cd "$d" || exit
    seq -w $NFILES | sed 's/^/0011223344/g' | xargs -P"$(nproc)" touch
    cd "$dir" || exit
}

benchmark() {
    create_files
    cd "$d" || exit
    # strace -f -c -o $dir/strace.txt $dir/brn2 -s -q -d . 2>&1
    $dir/brn2 -s -q -d .
    cd "$dir" || exit
}

valgrind2() {
    ls > rename

    vg_flags="--error-exitcode=1 --errors-for-leak-kinds=all"
    vg_flags="$vg_flags --leak-check=full --show-leak-kinds=all"

    valgrind $vg_flags $dir/brn2 -r . || exit
    valgrind $vg_flags $dir/brn2 -d . || exit
    valgrind $vg_flags $dir/brn2 -f rename || exit
}

target="${1:-build}"
PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

main="main.c"
program="brn2"
exe="$program"

CFLAGS="$CFLAGS -std=c99 -D_DEFAULT_SOURCE "
CFLAGS="$CFLAGS -Wextra -Wall -Wno-unused-macros -Wno-unused-function"
LDFLAGS="$LDFLAGS -lm -lpthread "

CC=${CC:-cc}
if [ "$CC" = "clang" ]; then
    CFLAGS="$CFLAGS -Weverything "
    CFLAGS="$CFLAGS -Wno-unsafe-buffer-usage -Wno-format-nonliteral "
    CFLAGS="$CFLAGS -Wno-disabled-macro-expansion "
    CFLAGS="$CFLAGS -Wno-constant-logical-operand "
fi

if [ "$CC" = "zig cc" ]; then
    case "$target" in
        "windows")
            CFLAGS="$CFLAGS -target x86_64-windows-gnu"
            CPPFLAGS="$CPPFLAGS "
            exe="$program.exe"
            ;;
        "mac-arm")
            CFLAGS="$CFLAGS -target aarch64-macos"
            CPPFLAGS="$CPPFLAGS "
            exe="$program"
            ;;
        "mac-x86")
            CFLAGS="$CFLAGS -target x86_64-macos"
            CPPFLAGS="$CPPFLAGS "
            exe="$program"
            ;;
        "openbsd")
            CFLAGS="$CFLAGS -target x86_64-openbsd-gnu "
            CPPFLAGS="$CPPFLAGS "
            exe="$program"
            ;;
        "freebsd")
            CFLAGS="$CFLAGS -v -target x86_64-freebsd "
            CFLAGS="$CFLAGS -isystem $dir/freebsd/usr/include "
            CFLAGS="$CFLAGS -isysroot $dir/freebsd/ --sysroot $dir/freebsd"
            CPPFLAGS="$CPPFLAGS "
            exe="$program"
            ;;
        *)
            echo "Invalid target for zig cc: $target"
            exit 1 ;;
    esac
fi

case "$target" in
    "debug")
        CFLAGS="$CFLAGS -g -fsanitize=undefined"
        CPPFLAGS="$CPPFLAGS -DBRN2_DEBUG=1" ;;
    "benchmark")
        CFLAGS="$CFLAGS    -O2 -flto -march=native -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK" ;;
    "valgrind") 
        CFLAGS="$CFLAGS -g -O2 -flto -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS -DBRN2_DEBUG" ;;
    "test") 
        CFLAGS="$CFLAGS -g -DBRN2_DEBUG"
        CPPFLAGS="$CPPFLAGS "
        ;;
    "mac-x86"|"mac-arm") 
        CFLAGS="$CFLAGS -fno-lto"
        CPPFLAGS="$CPPFLAGS "
        ;;
    "check") 
        CC=gcc
        CFLAGS="$CFLAGS -fanalyzer"
        CPPFLAGS="$CPPFLAGS "
        ;;
    *) 
        CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS -DBRN2_DEBUG=0" ;;
esac

case "$target" in
    "uninstall")
        set -x
        rm -f ${DESTDIR}${PREFIX}/bin/${program}
        rm -f ${DESTDIR}${PREFIX}/man/man1/${program}.1
        exit
        ;;
    "install")
        [ ! -f $program ] && $0 build
        set -x
        install -Dm755 ${program} ${DESTDIR}${PREFIX}/bin/${program}
        install -Dm644 ${program}.1 ${DESTDIR}${PREFIX}/man/man1/${program}.1
        exit
        ;;
    "assembly")
        $CC $CPPFLAGS $CFLAGS -S -o ${program}_$CC.S "$main" $LDFLAGS
        exit
        ;;
    "test")
        for src in *.c; do
            if [ "$src" = "$main" ]; then
                continue
            fi
            printf "Testing ${RED}${src}${RES} ...\n"
            name="$(echo "$src" | sed 's/\.c//g')"

            flags="$(awk '/\/\/ flags:/ { $1=$2=""; print $0 }' "$src")"
            cmdline="$CC $CPPFLAGS $CFLAGS"
            cmdline="$cmdline -D TESTING_$name=1 $src -o /tmp/$src.exe $flags"
            set -x
            if $cmdline; then
                /tmp/$src.exe || gdb /tmp/$src.exe
            else
                printf "Failed to compile ${RED} $src ${RES}.\n"
                exit
            fi

            set +x 
        done
        exit
        ;;
    *)
        set -x
        ctags --kinds-C=+l+d ./*.h ./*.c 2> /dev/null || true
        vtags.sed tags > .tags.vim 2> /dev/null || true
        $CC $CPPFLAGS $CFLAGS -o ${exe} "$main" $LDFLAGS
        ;;
esac

case "$target" in
    "benchmark") benchmark "$2" ;;
    "callgrind") callgrind "$2" ;;
    "valgrind") valgrind2 "$2" ;;
    "profile") profile "$2" ;;
    "check") scan-build --view -analyze-headers ./build.sh ;;
esac

set +x
if [ "$target" = "test_all" ]; then
    for t in $targets; do
        $0 $t
    done
fi
