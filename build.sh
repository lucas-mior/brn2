#!/bin/sh

# shellcheck disable=SC2086

targets='
test
build
debug
benchmark
valgrind
check
cross x86_64-windows-gnu
cross x86_64-macos
cross aarch64-macos
'

dir="$(realpath "$(dirname "$0")")"

target="${1:-build}"
cross="$2"

printf "$RED $0 $1 $2 $RES\n"
PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

main="main.c"
program="brn2"
exe="$program"

CFLAGS="$CFLAGS -std=c99"
CFLAGS="$CFLAGS -Wextra -Wall -Wno-unused-macros -Wno-unused-function"
CPPFLAGS="$CPPFLAGS -D_DEFAULT_SOURCE"
LDFLAGS="$LDFLAGS -lm"

CC=${CC:-cc}

case "$target" in
    "debug")
        CFLAGS="$CFLAGS -g -fsanitize=undefined"
        CPPFLAGS="$CPPFLAGS -DBRN2_DEBUG"
        ;;
    "benchmark")
        CFLAGS="$CFLAGS    -O2 -flto -march=native -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK"
        ;;
    "valgrind") 
        CFLAGS="$CFLAGS -g -O2 -flto -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS -DBRN2_DEBUG"
        ;;
    "test") 
        CFLAGS="$CFLAGS -g -DBRN2_DEBUG"
        CPPFLAGS="$CPPFLAGS "
        ;;
    "check") 
        CC=gcc
        CFLAGS="$CFLAGS -fanalyzer"
        CPPFLAGS="$CPPFLAGS "
        ;;
    *) 
        CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
        CPPFLAGS="$CPPFLAGS "
        ;;
esac

if [ "$target" = "cross" ]; then
    CC="zig cc"
    CFLAGS="$CFLAGS -target $cross"

    case $cross in
    "x86_64-macos"|"aarch64-macos")
        CFLAGS="$CFLAGS -fno-lto"
        LDFLAGS="$LDFLAGS -lpthread"
        ;;
    "x86_64-windows-gnu")
        exe="$program.exe"
        ;;
    *)
        LDFLAGS="$LDFLAGS -lpthread"
        ;;
    esac
else
    LDFLAGS="$LDFLAGS -lpthread"
fi

if [ "$CC" = "clang" ]; then
    CFLAGS="$CFLAGS -Weverything "
    CFLAGS="$CFLAGS -Wno-unsafe-buffer-usage -Wno-format-nonliteral "
    CFLAGS="$CFLAGS -Wno-disabled-macro-expansion "
    CFLAGS="$CFLAGS -Wno-constant-logical-operand "
fi

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
    "benchmark") 
        tmpdir="/tmp/brn2"
        rm -rf "$tmpdir"
        mkdir -p "$tmpdir"
        cd "$tmpdir" || exit

        seq -w 500000 | sed 's/^/0011223344/g' | xargs -P"$(nproc)" touch

        # strace -f -c -o $dir/strace.txt $dir/brn2 -s -q -d . 2>&1
        $dir/brn2 -s -q -d .
        exit
        ;;
    "valgrind") 
        ls > rename

        vg_flags="--error-exitcode=1 --errors-for-leak-kinds=all"
        vg_flags="$vg_flags --leak-check=full --show-leak-kinds=all"

        valgrind $vg_flags $dir/brn2 -r . || exit 1
        valgrind $vg_flags $dir/brn2 -d . || exit 1
        valgrind $vg_flags $dir/brn2 -f rename || exit 1
        exit
        ;;
    "check")
        scan-build --view -analyze-headers --status-bugs ./build.sh || exit 1
        exit
        ;;
esac

set +x
if [ "$target" = "test_all" ]; then
    printf '%s\n' "$targets" | while IFS= read -r t; do
        $0 $t || exit 1
    done
fi
