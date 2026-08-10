#!/bin/sh -e

# shellcheck disable=SC2086,SC2031 

dir=$(dirname "$(readlink -f "$0")")
cd "$dir" || exit

# shellcheck source=./cbase/common.sh
. ./cbase/common.sh

program=$(get_program "$0")
script=$(basename "$0")

target="${1:-debug}"

if ! grep -q "$target" ./targets; then
    echo "usage: $script <targets>"
    cat ./targets
    exit 1
fi

printf "\n${script} ${RED}${1:-} ${2:-}$RES\n"

PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

exe="bin/$program"
mkdir -p "$(dirname "$exe")"

OS=$(uname -a)

CC=$(get_compiler "$target")
case "$target" in
check)
    CC=gcc
    ;;
cross)
    CC="zig cc"
    ;;
esac
is_clang_cl=0
case "$CC" in
clang-cl|*/clang-cl)
    is_clang_cl=1
    ;;
esac

CPPFLAGS="$CPPFLAGS -I$dir/cbase"

CFLAGS="$CFLAGS -std=c11"
CFLAGS="$CFLAGS -Wfatal-errors"
CFLAGS="$CFLAGS -Wextra -Wall -Wpedantic"
# CFLAGS="$CFLAGS -Werror=all -Werror=extra"
# CFLAGS="$CFLAGS -Werror"  # Only uncomment occasionally, keep this line

if [ "$CC" = "clang" ] || [ "$CC" = "zig cc" ]; then
    CFLAGS="$CFLAGS -Weverything"
    CFLAGS="$CFLAGS -Wno-assign-enum"
    CFLAGS="$CFLAGS -Wno-c++-keyword"
    CFLAGS="$CFLAGS -Wno-cast-qual"
    CFLAGS="$CFLAGS -Wno-char-subscripts"
    CFLAGS="$CFLAGS -Wno-constant-logical-operand"
    CFLAGS="$CFLAGS -Wno-covered-switch-default"
    CFLAGS="$CFLAGS -Wno-disabled-macro-expansion"
    CFLAGS="$CFLAGS -Wno-float-equal"
    CFLAGS="$CFLAGS -Wno-format-nonliteral"
    CFLAGS="$CFLAGS -Wno-implicit-int-enum-cast"
    CFLAGS="$CFLAGS -Wno-implicit-void-ptr-cast"
    CFLAGS="$CFLAGS -Wno-pre-c11-compat"
    CFLAGS="$CFLAGS -Wno-unsafe-buffer-usage"
    CFLAGS="$CFLAGS -Wno-unused-macros"
    CFLAGS="$CFLAGS -Wno-used-but-marked-unused"
fi

case "$target" in
debug)
    CFLAGS="$CFLAGS -g3 -O0 -fsanitize=undefined"
    CPPFLAGS="$CPPFLAGS -DDEBUGGING=1 -Wno-unused-function"
    LDFLAGS="$LDFLAGS -lm"
    exe="bin/${program}_debug"
    ;;
benchmark)
    CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK=1"
    exe="bin/${program}_benchmark"
    ;;
perf)
    CFLAGS="$CFLAGS -g3 -Og -flto"
    CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK=1"
    exe="bin/${program}_perf"
    ;;
valgrind)
    CFLAGS="$CFLAGS -g3 -O0 -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS -DDEBUGGING=1"
    ;;
callgrind)
    CFLAGS="$CFLAGS -g3 -O2 -ftree-vectorize"
    ;;
test)
    CFLAGS="$CFLAGS -g3 -O0 -DDEBUGGING=1"
    LDFLAGS="$LDFLAGS -lm"
    ;;
check)
    CFLAGS="$CFLAGS -DDEBUGGING=1 -fanalyzer"
    LDFLAGS="$LDFLAGS -lm"
    ;;
build)
    CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
    ;;
fast_feedback)
    ;;
*)
    CFLAGS="$CFLAGS -O2"
    ;;
esac

if [ "$target" = "cross" ]; then
    cross="$2"
    if [ "$cross" = "all" ]; then
        status=0
        for f in $(awk '/^cross / { print $NF }' ./targets); do
            echo "running cross $f ..."
            if "$0" cross "$f"; then
                echo "ran cross $f ..."
            else
                status=1
                echo "failed cross $f ..."
            fi
        done
        exit "$status"
    fi
    CFLAGS="$CFLAGS -target $cross"

    case $cross in
    x86_64-macos|aarch64-macos)
        CPPFLAGS="$CPPFLAGS -D_DARWIN_C_SOURCE"
        LDFLAGS="$LDFLAGS -lpthread"
        ;;
    *linux*)
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
    case "$OS" in
    *Linux*)
        ;;
    *Darwin*)
        CPPFLAGS="$CPPFLAGS -D_DARWIN_C_SOURCE"
        ;;
    *MINGW*|*MSYS*|*CYGWIN*)
        ;;
    esac
    case "$OS" in
    *MINGW*|*MSYS*|*CYGWIN*)
        ;;
    *)
        LDFLAGS="$LDFLAGS -lpthread"
        ;;
    esac
fi

if [ "$is_clang_cl" -eq 1 ] && [ "$target" != "test" ]; then
    if [ -z "$CLANG_CL_TARGET" ]; then
        case "$OS" in
        *Linux*|*Darwin*|*BSD*)
            CLANG_CL_TARGET=$(cc -dumpmachine 2>/dev/null || true)
            ;;
        esac
    fi

    if [ -n "$CLANG_CL_TARGET" ]; then
        CFLAGS="$CFLAGS --target=$CLANG_CL_TARGET"
    fi

    CPPFLAGS=$(gcc_flags_to_msvc $CPPFLAGS)
    CFLAGS=$(gcc_flags_to_msvc $CFLAGS)
    LDFLAGS=$(gcc_flags_to_msvc $LDFLAGS)
fi

case "$target" in
fast_feedback)
    trace_on
    $CC $CPPFLAGS $CFLAGS main.c -o "$exe" $LDFLAGS && LC_ALL=C "$exe"
    trace_off
    exit
    ;;
uninstall)
    trace_on

    rm -f "${DESTDIR}${PREFIX}/bin/${program}"
    uninstall_opt "${program}.1" "${DESTDIR}${PREFIX}/man/man1/${program}.1"
    uninstall_opt "completions/${program}.bash" \
        "${DESTDIR}${PREFIX}/share/bash-completion/completions/${program}"
    uninstall_opt "completions/_${program}" \
        "${DESTDIR}${PREFIX}/share/zsh/site-functions/_${program}"
    uninstall_opt "completions/${program}.fish" \
        "${DESTDIR}${PREFIX}/share/fish/vendor_completions.d/${program}.fish"
    uninstall_opt "etc" "${DESTDIR}/etc/${program}"
    uninstall_opt \
        "${program}.desktop" "${DESTDIR}/usr/share/applications/${program}.desktop"

    trace_off
    exit
    ;;
install)
    trace_on

    if [ ! -f "$exe" ]; then
        "$0" build
    fi

    install -Dm755 "$exe" "${DESTDIR}${PREFIX}/bin/${program}"
    install_opt -Dm644 "${program}.1" "${DESTDIR}${PREFIX}/man/man1/${program}.1"
    install_opt -Dm644 "completions/${program}.bash" \
        "${DESTDIR}${PREFIX}/share/bash-completion/completions/${program}"
    install_opt -Dm644 "completions/_${program}" \
        "${DESTDIR}${PREFIX}/share/zsh/site-functions/_${program}"
    install_opt -Dm644 "completions/${program}.fish" \
        "${DESTDIR}${PREFIX}/share/fish/vendor_completions.d/${program}.fish"
    install_opt -dm755 "etc" "${DESTDIR}/etc/${program}"
    install_opt -Dm755 \
        "${program}.desktop" \
        "${DESTDIR}/usr/share/applications/${program}.desktop"

    trace_off
    exit
    ;;
assembly)
    trace_on
    $CC $CPPFLAGS $CFLAGS -S $LDFLAGS -o ${program}_$CC.S main.c
    exit
    ;;
test)
    test "$2"
    exit
    ;;
test_all)
    ;;
*)
    trace_on

    build_tags

    $CC $CPPFLAGS $CFLAGS -o ${exe} main.c $LDFLAGS

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
benchmark)
    create_temp_files
    ls > "rename"

    # strace -f -c -o $dir/strace.txt $dir/brn2 -s -q -d . 2>&1
    trace_on
    $dir/$exe -s -q -f "rename"
    trace_off
    rm $dir/$exe
    exit
    ;;
valgrind)
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
perf)
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
check)
    CC=gcc CFLAGS="-fanalyzer" ./build.sh

    CFLAGS="--analyze -Xanalyzer -analyzer-output=text"
    CFLAGS="$CFLAGS -Xanalyzer -analyzer-werror"
    CFLAGS="$CFLAGS -Xanalyzer -analyzer-opt-analyze-headers"
    CFLAGS="$CFLAGS -Wno-unused-command-line-argument"
    CC=clang CFLAGS="$CFLAGS" ./build.sh
    exit
    ;;
esac

trace_off
if [ "$target" = "test_all" ]; then
    while IFS= read -r target <&3; do
        echo "target=$target"

        echo "$target" | grep -Eq "^(# |$)" && continue

        if echo "$target" | grep -q "cross"; then
            $0 $target
            continue
        fi

        for compiler in clang gcc "zig cc"; do
            printf "\nCC=${RED}${compiler}${RES}\n"
            CC="$compiler" $0 "$target" || exit 3
        done
    done 3< ./targets
fi
