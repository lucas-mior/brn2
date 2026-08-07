#!/bin/sh -e

# shellcheck disable=SC2086,SC2031 

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
CPPFLAGS="$CPPFLAGS -I$dir/cbase"
cd "$dir" || exit
program=$(basename "$(readlink -f "$(dirname "$0")")")
script=$(basename "$0")

targets="$(sort ./targets)"
target="${1:-debug}"

if ! grep -q "$target" ./targets; then
    echo "usage: $script <targets>"
    cat ./targets
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
CFLAGS="$CFLAGS -Werror"
CFLAGS="$CFLAGS -Wno-format-pedantic"
CFLAGS="$CFLAGS -Wno-unknown-warning-option"
CFLAGS="$CFLAGS -Wno-gnu-union-cast"
CFLAGS="$CFLAGS -Wno-unused-macros"
CFLAGS="$CFLAGS -Wno-constant-logical-operand"
CFLAGS="$CFLAGS -Wno-tautological-constant-out-of-range-compare"
CFLAGS="$CFLAGS -Wno-float-equal"
CFLAGS="$CFLAGS -Wno-undefined-internal"
CFLAGS="$CFLAGS -Wno-cast-qual"
CFLAGS="$CFLAGS -Wno-unknown-pragmas"
CFLAGS="$CFLAGS -Wno-char-subscripts"

OS=$(uname -a)

case "$target" in
debug|test)
    CC="${CC:-tcc}"
    ;;
fast_feedback)
    CC="${CC:-clang}"
    ;;
*)
    CC="${CC:-cc}"
    ;;
esac

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
    CFLAGS="$CFLAGS -Wno-implicit-int-enum-cast"
    CFLAGS="$CFLAGS -Wno-assign-enum"
    CFLAGS="$CFLAGS -Wno-cast-function-type-strict"
    CFLAGS="$CFLAGS -Wno-bad-function-cast"
    CFLAGS="$CFLAGS -Wno-char-subscripts"
fi

if ! command xsel; then
    xsel=cat
else
    xsel=xsel
fi

if echo "$OS" | grep -q "Linux"; then
    if echo "$OS" | grep -q "GNU"; then
        GNUSOURCE="-D_GNU_SOURCE"
    fi
fi

option_remove() {
    echo "$1" | sed -E "s| *$2 +| |g"
}

with_toy_cc () {
    compiler="$1"
    compiler_macro=$(echo "$compiler" | tr '[:lower:]' '[:upper:]')
    compiler_macro="__${compiler_macro}__"
    shift
    args="$*"
    trace_on
    while ! problem=$($compiler \
                      "-D${compiler_macro}" -D__attribute=__attribute__ \
                      $args 2>&1); do
        trace_off
        problem=$(echo "$problem" | head -n 1 | tr -d "'")

        sleep 0.4
        if echo "$problem" | grep -Eq "unknown (argument|option)"; then
            arg=$(echo "$problem" | awk '{print $NF}')
            printf "\nRemoving argument $arg...\n"
            args=$(option_remove "$args" "$arg")
        elif echo "$problem" | grep -q "unknown file extension:"; then
            arg=$(echo "$problem" | awk '{print $NF}')
            printf "\nRemoving argument $arg...\n"
            args=$(option_remove "$args" "$arg")
        else
            printf "\n\nError compiling with $compiler:\n\n%s" "${problem}\n\n"
            return 1
        fi
        printf "\n"
        trace_on
    done
    return 0
}


case "$target" in
debug)
    CFLAGS="$CFLAGS -g3 -O0 -fsanitize=undefined"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DDEBUGGING=1 -Wno-unused-function"
    LDFLAGS="$LDFLAGS -lm"
    exe="bin/${program}_debug"
    ;;
benchmark)
    CFLAGS="$CFLAGS -O2 -flto -march=native -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DBRN2_BENCHMARK=1"
    exe="bin/${program}_benchmark"
    ;;
perf)
    CFLAGS="$CFLAGS -g3 -Og -flto"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DBRN2_BENCHMARK=1"
    exe="bin/${program}_perf"
    ;;
valgrind)
    CFLAGS="$CFLAGS -g3 -O0 -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DDEBUGGING=1"
    ;;
callgrind)
    CFLAGS="$CFLAGS -g3 -O2 -ftree-vectorize"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE"
    ;;
test)
    CFLAGS="$CFLAGS -g3 -O0 $GNUSOURCE -DDEBUGGING=1"
    CFLAGS="$CFLAGS -fsanitize=undefined -Wno-address"
    CFLAGS="$CFLAGS -Wno-unused-function"
    LDFLAGS="$LDFLAGS -lm"
    ;;
check)
    CC=gcc
    CFLAGS="$CFLAGS $GNUSOURCE -DDEBUGGING=1 -fanalyzer"
    LDFLAGS="$LDFLAGS -lm"
    ;;
build)
    CFLAGS="$CFLAGS $GNUSOURCE -O2 -flto -march=native -ftree-vectorize"
    ;;
fast_feedback)
    CFLAGS="$CFLAGS $GNUSOURCE -Werror"
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
    CC="zig cc"
    CFLAGS="$CFLAGS -target $cross"
    CFLAGS=$(option_remove "$CFLAGS" "-D_GNU_SOURCE")

    case $cross in
    "x86_64-macos"|"aarch64-macos")
        CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=700 -D_DARWIN_C_SOURCE"
        CFLAGS="$CFLAGS -fno-lto"
        LDFLAGS="$LDFLAGS -lpthread"
        ;;
    *linux*)
        CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=700"
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
        CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=700"
        ;;
    *Darwin*)
        CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=700 -D_DARWIN_C_SOURCE"
        ;;
    esac
    LDFLAGS="$LDFLAGS -lpthread"
fi

install_opt () {
    mode="$1"
    file="$2"
    dest="$3"

    if [ -f "$file" ]; then
        install "$mode" "$file" "$dest"
    elif [ -d "$file" ]; then
        install "$mode" "$dest"
        cp -rp "$file/." "$dest/"
    fi
}

uninstall_opt () {
    file="$1"
    dest="$2"

    if [ -e "$file" ]; then
        rm -rf "$dest"
    fi
}

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
    $CC $CPPFLAGS $CFLAGS -S $LDFLAGS -o ${program}_$CC.S "$main"
    exit
    ;;
test)
    find . -iname "*.c" | sort | while read -r src; do
        trace_off
        name=$(basename "$src")

        if [ -n "$2" ] && [ "$name" != "$2" ]; then
            continue
        fi
        if [ "$name" = "$main" ]; then
            continue
        fi
        if echo "$src" | grep -q "stc/"; then
            continue
        fi
        name=$(echo "$name" | sed 's/\.c//')
        test_exe="/tmp/${name}_test"

        printf "\nTesting ${RED}${src}${RES} ...\n"

        flags="$(awk '/\/\/ flags:/ { $1=$2=""; print $0 }' "$src")"
        if [ "$name" = "windows_functions" ]; then
            if ! zig version; then
                continue
            fi
            CC="zig cc"
            cmdline="$CC $CPPFLAGS $CFLAGS"
            cmdline=$(option_remove "$cmdline" "-D_GNU_SOURCE")
            cmdline="$cmdline -target x86_64-windows-gnu"
            cmdline="$cmdline -Wno-unused-variable"
            cmdline="$cmdline -DTESTING_$name=1 -DTESTING=1"
            cmdline="$cmdline $flags -o $test_exe $src"
        else
            cmdline="$CC $CPPFLAGS $CFLAGS"
            cmdline="$cmdline -Wno-unused-variable"
            cmdline="$cmdline -DTESTING_$name=1 -DTESTING=1"
            cmdline="$cmdline -o $test_exe $src $LDFLAGS $flags"
        fi

        if [ "$name" = "cbase_main_separate_object" ]; then
            cmdline=$(option_remove "$cmdline" "-DDEBUGGING=1")
        fi

        if [ "$CC" = "chibicc" ] || [ "$CC"  = "cproc" ]; then
            trace_on
            cmdline_no_cc=$(option_remove "$cmdline" "$CC")
            if with_toy_cc "$CC" "$cmdline_no_cc"; then
                /tmp/${name}_test
            else
                exit 1
            fi
        else
            trace_on
            if $cmdline; then
                if ! $test_exe; then
                    gdb --quiet \
                        -ex run -ex backtrace -ex quit \
                        $test_exe 2>&1 | $xsel -o -b
                    exit 1
                fi
            else
                exit 1
            fi
        fi
        trace_off
    done
    exit
    ;;
test_all)
    ;;
*)
    trace_on

    find . -iname "*.[ch]" -print0 \
        | xargs --verbose -0 ctags --kinds-C=+l+d || true
    vtags.sed tags | sort | uniq > .tags.vim      || true

    if [ "$CC" = "chibicc" ]; then
        with_toy_cc chibicc $CPPFLAGS $CFLAGS $LDFLAGS -o ${exe} "$main"
    elif [ "$CC" = "cproc" ]; then
        with_toy_cc cproc   $CPPFLAGS $CFLAGS $LDFLAGS -o ${exe} "$main"
    else
        $CC $CPPFLAGS $CFLAGS -o ${exe} "$main" $LDFLAGS
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

        for compiler in tcc clang gcc; do
            printf "\nCC=${RED}${compiler}${RES}\n"
            CC="$compiler" $0 "$target" || exit 3
        done
    done 3< ./targets
fi
