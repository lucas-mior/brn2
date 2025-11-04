#!/bin/sh -e

# shellcheck disable=SC2086

alias trace_on='set -x'
alias trace_off='{ set +x; } 2>/dev/null'

. ./targets

dir="$(realpath "$(dirname "$0")")"

target="${1:-build}"

if ! grep -q "$target" ./targets; then
    echo "usage: $(basename "$0") <targets>"
    cat targets
    exit 1
fi

cross="$2"

printf "\n${0} ${RED}${1} ${2}$RES\n"
PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

main="main.c"
program="brn2"
exe="bin/$program"
mkdir -p "$(dirname "$exe")"

CFLAGS="$CFLAGS -std=c99"
CFLAGS="$CFLAGS -Wextra -Wall"
CFLAGS="$CFLAGS -Wno-unknown-warning-option"
CFLAGS="$CFLAGS -Wno-unused-macros -Wno-unused-function"
CFLAGS="$CFLAGS -Wno-constant-logical-operand"
CFLAGS="$CFLAGS -Wno-unknown-pragmas"
CFLAGS="$CFLAGS -Wfatal-errors"
CPPFLAGS="$CPPFLAGS -D_DEFAULT_SOURCE"
LDFLAGS="$LDFLAGS -lm"

OS=$(uname -a)

if echo "$OS" | grep -q "Linux"; then
    if echo "$OS" | grep -q "GNU"; then
        GNUSOURCE="-D_GNU_SOURCE"
    fi
fi

option_remove() {
    echo "$1" | sed "s/$2//g"
}

CC=${CC:-cc}

case "$target" in
"debug")
    CFLAGS="$CFLAGS -g -fsanitize=undefined"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DDEBUGGING=1"
    exe="bin/${program}_debug"
    ;;
"benchmark")
    CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DBRN2_BENCHMARK=1"
    exe="bin/${program}_benchmark"
    ;;
"perf")
    CFLAGS="$CFLAGS -g3 -Og -flto"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DBRN2_BENCHMARK=1"
    exe="bin/${program}_perf"
    ;;
"valgrind") 
    CFLAGS="$CFLAGS -g -O1 -flto -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DDEBUGGING=1"
    ;;
"test")
    CFLAGS="$CFLAGS -g $GNUSOURCE -DDEBUGGING=1"
    ;;
"check") 
    CC=gcc
    CFLAGS="$CFLAGS $GNUSOURCE -fanalyzer"
    ;;
"build") 
    CFLAGS="$CFLAGS $GNUSOURCE -O2 -flto -march=native -ftree-vectorize"
    ;;
*)
    CFLAGS="$CFLAGS -O2"
    ;;
esac

if [ "$target" = "cross" ]; then
    CC="zig cc"
    CFLAGS="$CFLAGS -target $cross"
    CFLAGS=$(option_remove "$CFLAGS" "-D_GNU_SOURCE")

    case $cross in
    "x86_64-macos"|"aarch64-macos")
        CFLAGS="$CFLAGS -fno-lto"
        LDFLAGS="$LDFLAGS -lpthread"
        ;;
    *windows*)
        exe="bin/$program.exe"
        ;;
    *)
        LDFLAGS="$LDFLAGS -lpthread"
        ;;
    esac
else
    LDFLAGS="$LDFLAGS -lpthread"
fi

if [ "$target" != "test" ] && [ "$CC" = "clang" ]; then
    CFLAGS="$CFLAGS -Weverything"
    CFLAGS="$CFLAGS -Wno-unsafe-buffer-usage"
    CFLAGS="$CFLAGS -Wno-format-nonliteral"
    CFLAGS="$CFLAGS -Wno-disabled-macro-expansion"
    CFLAGS="$CFLAGS -Wno-c++-keyword"
    CFLAGS="$CFLAGS -Wno-implicit-void-ptr-cast"
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
    install -Dm755 bin/${program}   ${DESTDIR}${PREFIX}/bin/${program}
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
        if [ $src = "windows_functions.c" ]; then
            if ! zig version; then
                continue
            fi
            cmdline="zig cc $CPPFLAGS $CFLAGS"
            cmdline=$(option_remove "$cmdline" "-D_GNU_SOURCE")
            cmdline="$cmdline -target x86_64-windows-gnu"
            cmdline="$cmdline -Wno-unused-variable -DTESTING_$name=1"
            cmdline="$cmdline $src -o /tmp/$src.exe $flags"
        else
            cmdline="$CC $CPPFLAGS $CFLAGS -Wno-unused-variable -DTESTING_$name=1"
            cmdline="$cmdline $src -o /tmp/$src.exe $flags"
        fi

        trace_on
        if $cmdline; then
            /tmp/$src.exe || gdb /tmp/$src.exe -ex run
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
    vtags.sed tags > .tags.vim       2> /dev/null || true
    $CC $CPPFLAGS $CFLAGS -o ${exe} "$main" $LDFLAGS
    trace_off
    ;;
esac

create_temp_files() {
    tmpdir="/tmp/brn2"
    rm -rf "$tmpdir"
    mkdir -p "$tmpdir"
    cd "$tmpdir" || exit

    seq -w 100000 | sed 's/^/0011223344/g' | xargs -P"$(nproc)" touch
}

case "$target" in
"benchmark") 
    create_temp_files

    # strace -f -c -o $dir/strace.txt $dir/brn2 -s -q -d . 2>&1
    trace_on
    $dir/$exe -s -q -d .
    trace_off
    rm $dir/$exe
    exit
    ;;
"valgrind") 
    create_temp_files
    ls > rename

    vg_flags="--error-exitcode=1 --errors-for-leak-kinds=all"
    vg_flags="$vg_flags --leak-check=full --show-leak-kinds=all"

    trace_on
    find . \
    | valgrind --tool=helgrind $dir/bin/brn2 -f -
    valgrind --tool=helgrind $dir/bin/brn2 -d .
    valgrind --tool=helgrind $dir/bin/brn2 -f rename
    trace_off
    exit
    ;;
"perf")
    create_temp_files

    cd /tmp/brn2 || exit
    trace_on
    perf record -b -o $dir/perf.data $dir/$exe -s -q -d .
    cd "$dir"
    perf annotate $dir/$exe
    perf report -v perf.data
    trace_off
    exit
    ;;
"check")
    scan-build --view -analyze-headers --status-bugs ./build.sh
    exit
    ;;
esac

trace_off
if [ "$target" = "test_all" ]; then
    printf '%s\n' "$targets" | while IFS= read -r target; do
        echo "$target" | grep -Eq "^(# |$)" && continue
        if echo "$target" | grep "cross"; then
            $0 $target
            continue
        fi
        for compiler in gcc tcc clang "zig cc" ; do
            CC=$compiler $0 $target || exit
        done
    done
fi
