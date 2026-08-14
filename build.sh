#!/bin/sh -e

# shellcheck disable=SC2086,SC2031 

dir=$(dirname "$(readlink -f "$0")")
cd "$dir" || exit

# shellcheck source=./cbase/common.sh
. ./cbase/common.sh

project=$(common_get_program "$0")

common_build_parse_args "$@"

case "$mode" in
debug|benchmark|valgrind|callgrind)
    ;;
test|check|build|fast_feedback|cross|uninstall|install|test_all)
    ;;
*)
    common_build_unknown_mode
    ;;
esac

common_build_print_invocation "$project"

PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

exe="bin/$project"
mkdir -p "$(dirname "$exe")"

OS=$(uname -a)
CC=$(common_get_compiler "$mode")

is_msvc=0
is_clang_cl=0
is_cl=0
msvc_compiler=clang-cl

if [ "$mode" = "cross" ]; then
    if [ "$target" != "all" ]; then
        OS="$target"
    fi
fi

case "$OS" in
*MINGW*|*MSYS*|*CYGWIN*|*mingw*|*msys*|*cygwin*|*windows*)
    ;;
*)
    if [ "$is_msvc" -eq 0 ]; then
        CFLAGS="$CFLAGS -pthread"
    fi
esac

case "$CC" in
clang-cl|*/clang-cl)
    is_msvc=1
    is_clang_cl=1
    msvc_compiler=clang-cl
    ;;
cl|*/cl|cl.exe|*/cl.exe)
    is_msvc=1
    is_cl=1
    msvc_compiler=cl
    ;;
esac

CPPFLAGS="$CPPFLAGS -I. -Icbase"

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

case "$mode" in
debug)
    CFLAGS="$CFLAGS -g3 -Og"
    CPPFLAGS="$CPPFLAGS -DDEBUGGING=1 -Wno-unused-function"
    LDFLAGS="$LDFLAGS -lm"
    exe="bin/${project}_debug"
    ;;
benchmark)
    CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK=1"
    exe="bin/${project}_benchmark"
    ;;
valgrind)
    CFLAGS="$CFLAGS -g3 -O2 -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS -DDEBUGGING=1"
    ;;
callgrind)
    CFLAGS="$CFLAGS -g3 -O2 -ftree-vectorize"
    ;;
test)
    CFLAGS="$CFLAGS -g3 -Og -DDEBUGGING=1"
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
    CFLAGS="$CFLAGS -Werror"
    ;;
cross)
    common_build_cross_all
    cross="$target"

    CFLAGS="$CFLAGS -Wno-padded"
    CFLAGS="$CFLAGS -target $cross"

    case "$cross" in
    *windows*)
        exe="bin/$project.exe"
        ;;
    *)
        ;;
    esac
    ;;
uninstall|install|test_all)
    ;;
*)
    common_build_unknown_mode
    ;;
esac
if [ "$is_cl" -eq 1 ]; then
    case "$exe" in
    *.exe)
        ;;
    *)
        exe="$exe.exe"
        ;;
    esac
fi

if [ "$is_msvc" -eq 1 ] && [ "$mode" != "test" ]; then
    if [ -z "$CLANG_CL_TARGET" ]; then
        case "$OS" in
        *Linux*|*Darwin*|*BSD*)
            if [ "$is_clang_cl" -eq 1 ]; then
                CLANG_CL_TARGET=$(cc -dumpmachine 2>/dev/null || true)
            fi
            ;;
        esac
    fi

    if [ "$is_clang_cl" -eq 1 ] && [ -n "$CLANG_CL_TARGET" ]; then
        CFLAGS="$CFLAGS --target=$CLANG_CL_TARGET"
    fi

    CPPFLAGS=$(common_gcc_flags_to_msvc "$msvc_compiler" $CPPFLAGS)
    CFLAGS=$(common_gcc_flags_to_msvc "$msvc_compiler" $CFLAGS)
    LDFLAGS=$(common_gcc_flags_to_msvc "$msvc_compiler" $LDFLAGS)
fi

case "$mode" in
fast_feedback)
    trace_on
    $CC $CPPFLAGS $CFLAGS main.c -o "$exe" $LDFLAGS
    trace_off
    exit
    ;;
uninstall)
    trace_on

    rm -f "${DESTDIR}${PREFIX}/bin/${project}"
    common_uninstall_opt "${project}.1" "${DESTDIR}${PREFIX}/man/man1/${project}.1"
    common_uninstall_opt "completions/${project}.bash" \
        "${DESTDIR}${PREFIX}/share/bash-completion/completions/${project}"
    common_uninstall_opt "completions/_${project}" \
        "${DESTDIR}${PREFIX}/share/zsh/site-functions/_${project}"
    common_uninstall_opt "completions/${project}.fish" \
        "${DESTDIR}${PREFIX}/share/fish/vendor_completions.d/${project}.fish"
    common_uninstall_opt "etc" "${DESTDIR}/etc/${project}"
    common_uninstall_opt \
        "${project}.desktop" "${DESTDIR}/usr/share/applications/${project}.desktop"

    trace_off
    exit
    ;;
install)
    trace_on

    if [ ! -f "$exe" ]; then
        "$0" build
    fi

    install -Dm755 "$exe" "${DESTDIR}${PREFIX}/bin/${project}"
    common_install_opt -Dm644 "${project}.1" "${DESTDIR}${PREFIX}/man/man1/${project}.1"
    common_install_opt -Dm644 "completions/${project}.bash" \
        "${DESTDIR}${PREFIX}/share/bash-completion/completions/${project}"
    common_install_opt -Dm644 "completions/_${project}" \
        "${DESTDIR}${PREFIX}/share/zsh/site-functions/_${project}"
    common_install_opt -Dm644 "completions/${project}.fish" \
        "${DESTDIR}${PREFIX}/share/fish/vendor_completions.d/${project}.fish"
    common_install_opt -dm755 "etc" "${DESTDIR}/etc/${project}"
    common_install_opt -Dm755 \
        "${project}.desktop" \
        "${DESTDIR}/usr/share/applications/${project}.desktop"

    trace_off
    exit
    ;;
test)
    rm -rf /tmp/brn2* || true
    common_test "$target"
    exit
    ;;
cross)
    trace_on
    $CC $CPPFLAGS $CFLAGS -o ${exe} main.c $LDFLAGS
    trace_off
    ;;
benchmark|build|callgrind|debug|valgrind)
    common_build_tags
    trace_on

    if [ "$is_cl" -eq 1 ]; then
        $CC $CPPFLAGS $CFLAGS /Fe${exe} main.c $LDFLAGS
    else
        $CC $CPPFLAGS $CFLAGS -o ${exe} main.c $LDFLAGS
    fi

    trace_off
    ;;
check)
    set +e
    CC=gcc CFLAGS="-fanalyzer" ./build.sh

    CFLAGS="--analyze -Xanalyzer -analyzer-output=text"
    CFLAGS="$CFLAGS -Xanalyzer -analyzer-werror"
    CFLAGS="$CFLAGS -Xanalyzer -analyzer-opt-analyze-headers"
    CFLAGS="$CFLAGS -Wno-unused-command-line-argument"
    CC=clang CFLAGS="$CFLAGS" ./build.sh
    exit
    ;;
test_all)
    common_build_test_all "debug build test" \
        tcc clang gcc /opt/msvc/bin/x64/cl.exe
    ;;
*)
    ;;
esac

# brn2 specific stuff below

create_temp_files() {
    tmpdir="/tmp/brn2"
    rm -rf "$tmpdir"
    mkdir -p "$tmpdir"
    cd "$tmpdir" || exit

    seq -w 200000 | sed 's/^/0011223344/g' | xargs -P"$(nproc)" touch
}

valgrind_rotate_left() {
    input=$1
    output=$2

    sed '1d' "$input" > "$output"
    sed -n '1p' "$input" >> "$output"
}

valgrind_rotate_right() {
    input=$1
    output=$2

    tail -n 1 "$input" > "$output"
    sed '$d' "$input" >> "$output"
}

case "$mode" in
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
callgrind|valgrind)
    create_temp_files

    if [ "$mode" = "valgrind" ]; then
        tool="memcheck"
    else
        tool="$mode"
    fi

    valgrind_lists="/tmp/brn2-valgrind"
    rm -rf "$valgrind_lists"
    mkdir -p "$valgrind_lists"

    original="$valgrind_lists/original"
    shuffled="$valgrind_lists/shuffled"
    rotated_left="$valgrind_lists/rotated-left"
    rotated_right="$valgrind_lists/rotated-right"

    LC_ALL=C ls > "$original"
    shuf "$original" > "$shuffled"
    valgrind_rotate_left "$original" "$rotated_left"
    valgrind_rotate_right "$original" "$rotated_right"

    trace_on

    # cat "$original" \
    # | valgrind --log-file="$dir/valgrind.1.txt" -s --tool=$tool \
    #     "$dir/$exe" -q -f - --file-target "$shuffled"
    valgrind --log-file="$dir/valgrind.2.txt" -s --tool=$tool \
        "$dir/$exe" -q -d . --file-target "$rotated_left"
    valgrind --log-file="$dir/valgrind.3.txt" -s --tool=$tool \
        "$dir/$exe" -q -f "$original" --file-target "$rotated_right"

    trace_off
    exit
    ;;
esac
