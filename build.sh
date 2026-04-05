#!/bin/sh -e

# shellcheck disable=SC2086

set -e

error () {
    >&2 printf "$@"
    return
}

if [ -n "$BASH_VERSION" ]; then
    # shellcheck disable=SC3044
    shopt -s expand_aliases
fi

alias trace_on='set -x'
alias trace_off='{ set +x; } 2>/dev/null'


dir=$(dirname "$(readlink -f "$0")")
cbase="cbase"
CPPFLAGS="$CPPFLAGS -I$dir/$cbase"
cd "$dir" || exit
program=$(basename "$(readlink -f "$(dirname "$0")")")
script=$(basename "$0")

. ./targets
target="${1:-build}"

if ! grep -q "$target" ./targets; then
    echo "usage: $script <targets>"
    cat targets
    exit 1
fi

printf "\n${script} ${RED}${1} ${2}$RES\n"

PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

main="main.c"
exe="bin/$program"
mkdir -p "$(dirname "$exe")"

CPPFLAGS="$CPPFLAGS -D_DEFAULT_SOURCE"
CFLAGS="$CFLAGS -std=c11"
CFLAGS="$CFLAGS -Wfatal-errors"
CFLAGS="$CFLAGS -Wextra -Wall"
# CFLAGS="$CFLAGS -Werror"
CFLAGS="$CFLAGS -Wno-format-pedantic"
CFLAGS="$CFLAGS -Wno-unknown-warning-option"
CFLAGS="$CFLAGS -Wno-gnu-union-cast"
CFLAGS="$CFLAGS -Wno-unused-macros"
CFLAGS="$CFLAGS -Wno-unused-function"
CFLAGS="$CFLAGS -Wno-constant-logical-operand"
CFLAGS="$CFLAGS -Wno-float-equal"
CFLAGS="$CFLAGS -Wno-undefined-internal"
CFLAGS="$CFLAGS -Wno-cast-qual"
CFLAGS="$CFLAGS -Wno-unknown-pragmas"
LDFLAGS="$LDFLAGS -lm"

OS=$(uname -a)

CC="${CC:-cc}"
if echo "$OS" | grep -q "Linux"; then
    if echo "$OS" | grep -q "GNU"; then
        GNUSOURCE="-D_GNU_SOURCE"
    fi
fi

option_remove() {
    echo "$1" | sed -E "s| *$2 +| |g"
}

compile_with_chibicc () {
    args="$*"
    while ! problem=$(chibicc $args 2>&1); do
        trace_off
        sleep 0.4
        if echo "$problem" | grep -q "unknown argument:"; then
            arg=$(echo "$problem" | awk '{print $NF}')
            printf "\nRemoving argument $arg...\n"
            args=$(option_remove "$args" "$arg")
        elif echo "$problem" | grep -q "unknown file extension:"; then
            arg=$(echo "$problem" | awk '{print $NF}')
            printf "\nRemoving argument $arg...\n"
            args=$(option_remove "$args" "$arg")
        else
            printf "\n\nError compiling with chibicc:\n\n${problem}\n\n"
            return 1
        fi
        printf "\n"
        trace_on
    done
    return 0
}

case "$target" in
"debug")
    CFLAGS="$CFLAGS -g3 -fsanitize=undefined"
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
    CFLAGS="$CFLAGS -g -O0 -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DDEBUGGING=1"
    ;;
"callgrind")
    CFLAGS="$CFLAGS -g -O2 -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE"
    ;;
"test")
    CFLAGS="$CFLAGS -g $GNUSOURCE -DDEBUGGING=1 -fsanitize=undefined"
    ;;
"check")
    CC=gcc
    CFLAGS="$CFLAGS $GNUSOURCE -DDEBUGGING=1 -fanalyzer"
    ;;
"build")
    CFLAGS="$CFLAGS $GNUSOURCE -O2 -flto -march=native -ftree-vectorize"
    ;;
"release")
    CFLAGS="$CFLAGS $GNUSOURCE -DRELEASING=1 -O2 -flto -march=native -ftree-vectorize"
    ;;
"fast_feedback")
    CC=clang
    CFLAGS="$CFLAGS $GNUSOURCE -Werror"
    ;;

*)
    CFLAGS="$CFLAGS -O2"
    ;;
esac

if [ "$target" = "cross" ]; then
    cross="$2"
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

if [ "$CC" = "clang" ]; then
    CFLAGS="$CFLAGS -Weverything"
    CFLAGS="$CFLAGS -Wno-unsafe-buffer-usage"
    CFLAGS="$CFLAGS -Wno-format-nonliteral"
    CFLAGS="$CFLAGS -Wno-disabled-macro-expansion"
    CFLAGS="$CFLAGS -Wno-c++-keyword"
    CFLAGS="$CFLAGS -Wno-pre-c11-compat"
    CFLAGS="$CFLAGS -Wno-implicit-void-ptr-cast"
    CFLAGS="$CFLAGS -Wno-ignored-attributes"
    CFLAGS="$CFLAGS -Wno-covered-switch-default"
    CFLAGS="$CFLAGS -Wno-used-but-marked-unused"
    CFLAGS="$CFLAGS -Wno-c23-extensions"
    CFLAGS="$CFLAGS -Wno-implicit-int-enum-cast"
    CFLAGS="$CFLAGS -Wno-assign-enum"
fi


case "$target" in
"fast_feedback")
    trace_on
    $CC $CPPFLAGS $CFLAGS main.c -o "$exe" $LDFLAGS && LC_ALL=C "$exe"
    trace_off
    ;;
"uninstall")
    trace_on
    rm -f ${DESTDIR}${PREFIX}/bin/${program}
    rm -f ${DESTDIR}${PREFIX}/man/man1/${program}.1
    exit
    ;;
"install")
    trace_on
    if [ ! -f "$program" ]; then
        $0 build
    fi
    install -Dm755 bin/${program}   ${DESTDIR}${PREFIX}/bin/${program}
    install -Dm644 ${program}.1 ${DESTDIR}${PREFIX}/man/man1/${program}.1
    if [ -d "etc" ]; then
        install -dm755 "$DESTDIR/etc/$program"
        cp -rp etc/* "$DESTDIR/etc/$program/"
    fi
    if [ -f "$program.desktop" ]; then
        install -Dm755 \
            "$program.desktop" \
            "$DESTDIR/usr/share/applications/$program.desktop"
    fi
    trace_off
    exit
    ;;
"assembly")
    trace_on
    $CC $CPPFLAGS $CFLAGS -S $LDFLAGS -o ${program}_$CC.S "$main"
    exit
    ;;
"test")
    for src in *.c; do
        if [ -n "$2" ] && [ $src != "$2" ]; then
            continue
        fi
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
            cmdline="$cmdline $flags -o /tmp/$src.exe $src"
        else
            cmdline="$CC $CPPFLAGS $CFLAGS -Wno-unused-variable -DTESTING_$name=1"
            cmdline="$cmdline $flags -o /tmp/$src.exe $src"
        fi

        trace_on
        if [ $src != "windows_functions.c" ] && [ "$CC" = "chibicc" ]; then
            cmdline=$(option_remove "$cmdline" "$CC")
            compile_with_chibicc $cmdline && /tmp/$src.exe
        elif $cmdline; then
            /tmp/$src.exe || gdb /tmp/$src.exe -ex run
        else
            trace_off
            exit 1
        fi
        trace_off
        if [ -n "$2" ] && [ $src = "$2" ]; then
            exit
        fi
    done
    exit
    ;;
"test_all")
    ;;
*)
    trace_on
    ctags --kinds-C=+l+d ./*.h ./*.c 2> /dev/null || true
    vtags.sed tags | sort | uniq > .tags.vim       2> /dev/null || true
    if [ "$CC" = "chibicc" ]; then
        compile_with_chibicc $CPPFLAGS $CFLAGS $LDFLAGS -o ${exe} "$main"
    else
        $CC $CPPFLAGS $CFLAGS $LDFLAGS -o ${exe} "$main"
    fi
    trace_off
    ;;
esac

create_temp_files() {
    tmpdir="/tmp/brn2"
    rm -rf "$tmpdir"
    mkdir -p "$tmpdir"
    cd "$tmpdir" || exit

    seq -w 2000000 | sed 's/^/0011223344/g' | xargs -P"$(nproc)" touch
}

case "$target" in
"benchmark")
    create_temp_files
    ls > "rename"

    # strace -f -c -o $dir/strace.txt $dir/brn2 -s -q -d . 2>&1
    trace_on
    $dir/$exe -s -q -f "rename"
    trace_off
    rm $dir/$exe
    exit
    ;;
"valgrind")
    create_temp_files
    ls > rename

    vg_flags="--error-exitcode=1 --errors-for-leak-kinds=all"
    vg_flags="$vg_flags --leak-check=full --show-leak-kinds=all"
    vg_flags="$vg_flags --track-origins=yes"

    trace_on
    find . \
    | valgrind $vg_flags -s --tool=memcheck $dir/bin/brn2 -f -
    valgrind   $vg_flags -s --tool=memcheck $dir/bin/brn2 -d .
    valgrind   $vg_flags -s --tool=memcheck $dir/bin/brn2 -f rename
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
    CC=gcc CFLAGS="-fanalyzer" ./build.sh
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
        for compiler in chibicc gcc tcc clang "zig cc" ; do
            CC=$compiler $0 $target || exit
        done
    done
fi
