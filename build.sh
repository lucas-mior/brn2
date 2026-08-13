#!/bin/sh -e

# shellcheck disable=SC2086,SC2031 

dir=$(dirname "$(readlink -f "$0")")
cd "$dir" || exit

# shellcheck source=./cbase/common.sh
. ./cbase/common.sh

program=$(common_get_program "$0")
script=$(basename "$0")

common_build_parse_args "$@"
targets=$(cat ./targets)
common_build_validate_mode "$script" "$targets"

common_build_print_invocation "$script"

PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

exe="bin/$program"
mkdir -p "$(dirname "$exe")"

OS=$(uname -a)
CC=$(common_get_compiler "$mode")

is_msvc=0
is_clang_cl=0
is_cl=0
msvc_compiler=clang-cl

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

case "$mode" in
debug)
    CFLAGS="$CFLAGS -g3 -Og"
    CPPFLAGS="$CPPFLAGS -DDEBUGGING=1 -Wno-unused-function"
    LDFLAGS="$LDFLAGS -lm"
    exe="bin/${program}_debug"
    ;;
benchmark)
    CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS -DBRN2_BENCHMARK=1"
    exe="bin/${program}_benchmark"
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
*)
    CFLAGS="$CFLAGS -O2"
    ;;
esac

if [ "$mode" = "cross" ]; then
    cross="$target"
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

    rm -f "${DESTDIR}${PREFIX}/bin/${program}"
    common_uninstall_opt "${program}.1" "${DESTDIR}${PREFIX}/man/man1/${program}.1"
    common_uninstall_opt "completions/${program}.bash" \
        "${DESTDIR}${PREFIX}/share/bash-completion/completions/${program}"
    common_uninstall_opt "completions/_${program}" \
        "${DESTDIR}${PREFIX}/share/zsh/site-functions/_${program}"
    common_uninstall_opt "completions/${program}.fish" \
        "${DESTDIR}${PREFIX}/share/fish/vendor_completions.d/${program}.fish"
    common_uninstall_opt "etc" "${DESTDIR}/etc/${program}"
    common_uninstall_opt \
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
    common_install_opt -Dm644 "${program}.1" "${DESTDIR}${PREFIX}/man/man1/${program}.1"
    common_install_opt -Dm644 "completions/${program}.bash" \
        "${DESTDIR}${PREFIX}/share/bash-completion/completions/${program}"
    common_install_opt -Dm644 "completions/_${program}" \
        "${DESTDIR}${PREFIX}/share/zsh/site-functions/_${program}"
    common_install_opt -Dm644 "completions/${program}.fish" \
        "${DESTDIR}${PREFIX}/share/fish/vendor_completions.d/${program}.fish"
    common_install_opt -dm755 "etc" "${DESTDIR}/etc/${program}"
    common_install_opt -Dm755 \
        "${program}.desktop" \
        "${DESTDIR}/usr/share/applications/${program}.desktop"

    trace_off
    exit
    ;;
test)
    rm -rf /tmp/brn2* || true
    common_test "$target"
    exit
    ;;
test_all)
    ;;
*)

    common_build_tags
    trace_on

    if [ "$is_cl" -eq 1 ]; then
        $CC $CPPFLAGS $CFLAGS /Fe${exe} main.c $LDFLAGS
    else
        $CC $CPPFLAGS $CFLAGS -o ${exe} main.c $LDFLAGS
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
valgrind)
    create_temp_files

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

    cat "$original" \
    | valgrind --log-file="$dir/valgrind.1.txt" -s --tool=memcheck \
        "$dir/$exe" -q -f - --file-test "$shuffled"
    valgrind --log-file="$dir/valgrind.2.txt" -s --tool=memcheck \
        "$dir/$exe" -q -d . --file-test "$rotated_left"
    valgrind --log-file="$dir/valgrind.3.txt" -s --tool=memcheck \
        "$dir/$exe" -q -f "$original" --file-test "$rotated_right"

    trace_off
    exit
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
esac

trace_off
if [ "$mode" = "test_all" ]; then
    while IFS= read -r build_target <&3; do
        echo "target=$build_target"

        echo "$build_target" | grep -Eq "^(# |$)" && continue

        if echo "$build_target" | grep -q "cross"; then
            $0 $build_target
            continue
        fi

        for compiler in tcc clang gcc /opt/msvc/bin/x64/cl.exe; do
            printf "\nCC=${RED}${compiler}${RES}\n"
            CC="$compiler" $0 "$build_target" || exit 3
        done
    done 3< ./targets
fi
