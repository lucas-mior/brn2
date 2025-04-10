#!/bin/sh -e

# shellcheck disable=SC2086

alias trace_on='set -x'
alias trace_off='{ set +x; } 2>/dev/null'

targets='
test
build
debug
benchmark
perf
valgrind
check
cross x86_64-windows-gnu
cross x86_64-macos
cross aarch64-macos
'

dir="$(realpath "$(dirname "$0")")"

target="${1:-build}"
cross="$2"

printf "\n${0} ${RED}${1} ${2}$RES\n"
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
"benchmark"|"perf")
    CFLAGS="$CFLAGS    -O2 -flto -march=native -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK"
    exe="${program}_benchmark"
    ;;
"valgrind") 
    CFLAGS="$CFLAGS -g -O2 -flto -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS -DBRN2_DEBUG"
    ;;
"test") 
    CFLAGS="$CFLAGS -g -DBRN2_DEBUG"
    ;;
"check") 
    CC=gcc
    CFLAGS="$CFLAGS -fanalyzer"
    ;;
"build") 
    CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
    ;;
*)
    CFLAGS="$CFLAGS -O2"
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
    CFLAGS="$CFLAGS -Weverything"
    CFLAGS="$CFLAGS -Wno-unsafe-buffer-usage -Wno-format-nonliteral"
    CFLAGS="$CFLAGS -Wno-disabled-macro-expansion -Wno-constant-logical-operand"
fi

case "$target" in
"uninstall")
    trace_on
    rm -f ${DESTDIR}${PREFIX}/bin/${program}
    rm -f ${DESTDIR}${PREFIX}/man/man1/${program}.1
    exit
    ;;
"install")
    [ ! -f $program ] && $0 build
    trace_on
    install -Dm755 ${program} ${DESTDIR}${PREFIX}/bin/${program}
    install -Dm644 ${program}.1 ${DESTDIR}${PREFIX}/man/man1/${program}.1
    exit
    ;;
"assembly")
    trace_on
    $CC $CPPFLAGS $CFLAGS -S -o ${program}_$CC.S "$main" $LDFLAGS
    exit
    ;;
"test")
    for src in *.c; do
        if [ "$src" = "$main" ]; then
            continue
        fi
        printf "\nTesting ${RED}${src}${RES} ...\n"
        name="$(echo "$src" | sed 's/\.c//g')"

        flags="$(awk '/\/\/ flags:/ { $1=$2=""; print $0 }' "$src")"
        cmdline="$CC $CPPFLAGS $CFLAGS"
        cmdline="$cmdline -D TESTING_$name=1 $src -o /tmp/$src.exe $flags"

        trace_on
        if $cmdline; then
            /tmp/$src.exe || gdb /tmp/$src.exe
        else
            trace_off
            exit 1
        fi
        trace_off

    done
    exit
    ;;
"test_all")
    ;;
*)
    trace_on
    ctags --kinds-C=+l+d ./*.h ./*.c 2> /dev/null || true
    vtags.sed tags > .tags.vim 2> /dev/null || true
    $CC $CPPFLAGS $CFLAGS -o ${exe} "$main" $LDFLAGS
    trace_off
    ;;
esac

tempfiles() {
    tmpdir="/tmp/brn2"
    rm -rf "$tmpdir"
    mkdir -p "$tmpdir"
    cd "$tmpdir" || exit

    seq -w 500000 | sed 's/^/0011223344/g' | xargs -P"$(nproc)" touch
}

case "$target" in
"benchmark") 
    tempfiles

    # strace -f -c -o $dir/strace.txt $dir/brn2 -s -q -d . 2>&1
    $dir/$exe -s -q -d .
    exit
    ;;
"valgrind") 
    ls > rename

    vg_flags="--error-exitcode=1 --errors-for-leak-kinds=all"
    vg_flags="$vg_flags --leak-check=full --show-leak-kinds=all"

    valgrind $vg_flags $dir/brn2 -r .
    valgrind $vg_flags $dir/brn2 -d .
    valgrind $vg_flags $dir/brn2 -f rename
    exit
    ;;
"perf")
    tempfiles

    perf record -o $dir/perf.data $dir/$exe -s -q -d .
    exit
    ;;
"check")
    scan-build --view -analyze-headers --status-bugs ./build.sh
    exit
    ;;
esac

trace_off
if [ "$target" = "test_all" ]; then
    printf '%s\n' "$targets" | while IFS= read -r t; do
        echo "$t" | grep -Eq "^(# |$)" && continue
        $0 $t
    done
fi
