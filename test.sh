#!/bin/sh

set -e

test_target=${BRN2_TEST_TARGET:-debug}

./build.sh "$test_target"

case "$test_target" in
"debug")
    brn2="$PWD/bin/brn2_debug"
    ;;
"build"|"release")
    brn2="$PWD/bin/brn2"
    ;;
*)
    echo "Unsupported BRN2_TEST_TARGET: $test_target"
    exit 1
    ;;
esac

run_brn2 () {
    if command -v gdb >/dev/null 2>&1; then
        gdb -q -nx -batch --return-child-result \
            -ex run \
            --args "$brn2" "$@"
    else
        "$brn2" "$@"
    fi
}

if [ ! -d /tmp/brn2 ]; then
    mkdir /tmp/brn2
fi
cd /tmp/brn2

rm -rf "rename" "rename2"

for f in a b c d; do
    echo "$f" >  "$f"
    echo "$f" >> "rename"
done
echo "a" >> "rename"

for f in b c d a; do
    echo "$f" >> "rename2"
done

set -x
run_brn2 -f "rename" -t "rename2"

check () {
    if ! grep -q "^${1}$" "$2"; then
        echo "file $2 should be $1"
        exit 1
    fi
}
set +x

check a b
check b c
check c d
check d a

rm -rf "rename" "rename2"

for f in a b c d; do
    echo "$f" >  "$f"
    echo "$f" >> "rename"
done
echo "a" >> "rename"

for f in c bxx d a; do
    echo "$f" >> "rename2"
done

set -x
run_brn2 -f "rename" -t "rename2"
set +x

check a c
check b bxx
check c d
check d a

rm -f a b c d bxx
rm -rf "rename" "rename2"

for f in a b c d; do
    echo "$f" > "$f"
done

# Keep four unique names after filtering, but make the unfiltered list large
# enough that its precomputed indexes exceed the filtered hash set allocation.
awk 'BEGIN {
    for (i = 0; i < 16384; i += 1) {
        printf "a\nb\nc\nd\n"
    }
    print "a"
}' > "rename"

for f in b c d a; do
    echo "$f" >> "rename2"
done

set -x
run_brn2 -q -f "rename" -t "rename2" 2>/dev/null
set +x

check a b
check b c
check c d
check d a

rm -f a b c d bxx
rm -f "rename" "rename2"

check_unchanged () {
    expected=$1
    shift

    for file in "$@"; do
        if ! cmp -s "$expected/$file" "$file"; then
            echo "file $file changed after rejected rename list"
            exit 1
        fi
    done
}

expect_failure () {
    message=$1
    shift

    set +e
    printf '+ %s' "$brn2"
    for arg in "$@"; do
        printf ' %s' "$arg"
    done
    printf '\n'
    output=$({ "$brn2" "$@"; } 2>&1)
    status=$?
    set -e

    if [ -n "$output" ]; then
        printf '%s\n' "$output" \
        | sed \
            -e '/^Illegal instruction/d' \
            -e 's/[[:space:]]*Illegal instruction.*$//'
    fi

    if [ "$status" -eq 0 ]; then
        echo "$message"
        exit 1
    fi
}

rm -rf "autosolve-tests"
mkdir -p "autosolve-tests"
cd "autosolve-tests"

mkdir -p "unequal/expected"
cd "unequal"

printf "source a\n" > "expected/a"
printf "source b\n" > "expected/b"
cp "expected/a" "a"
cp "expected/b" "b"
printf "a\nb\n" > "rename"
printf "b\nb\n" > "rename2"

# A moving claimant may replace an unchanged owner only when the two source
# files are equal. Unequal files must be rejected without changing either one.
expect_failure \
    "--autosolve accepted unequal files with the same target" \
    --fatal --autosolve -f "rename" -t "rename2"
check_unchanged "expected" a b

cd ..
mkdir -p "equal-forward"
cd "equal-forward"

printf "same contents\n" > "a"
printf "same contents\n" > "b"
printf "a\nb\n" > "rename"
printf "b\nb\n" > "rename2"

run_brn2 --fatal --autosolve -f "rename" -t "rename2"

if [ -e "a" ]; then
    echo "--autosolve did not remove the equal moving source"
    exit 1
fi
if ! printf "same contents\n" | cmp -s - "b"; then
    echo "--autosolve changed the replacement target contents"
    exit 1
fi

cd ..
mkdir -p "equal-disabled/expected"
cd "equal-disabled"

printf "same contents\n" > "expected/a"
printf "same contents\n" > "expected/b"
cp "expected/a" "a"
cp "expected/b" "b"
printf "a\nb\n" > "rename"
printf "b\nb\n" > "rename2"

expect_failure \
    "duplicate targets succeeded without --autosolve" \
    --fatal -f "rename" -t "rename2"
check_unchanged "expected" a b

cd ..
mkdir -p "valid-before-invalid/expected"
cd "valid-before-invalid"

printf "same valid contents\n" > "expected/a"
printf "same valid contents\n" > "expected/b"
printf "unequal source c\n" > "expected/c"
printf "unequal source d\n" > "expected/d"
cp "expected/a" "a"
cp "expected/b" "b"
cp "expected/c" "c"
cp "expected/d" "d"
printf "a\nb\nc\nd\n" > "rename"
printf "b\nb\nd\nd\n" > "rename2"

# Verification must finish before execution. The valid a -> b replacement
# precedes the invalid c -> d conflict and therefore must not be performed.
expect_failure \
    "--autosolve executed a valid conflict before rejecting a later one" \
    --fatal --autosolve -f "rename" -t "rename2"
check_unchanged "expected" a b c d

cd ..
mkdir -p "equal-reversed"
cd "equal-reversed"

printf "same contents\n" > "a"
printf "same contents\n" > "b"
printf "b\na\n" > "rename"
printf "b\nb\n" > "rename2"

run_brn2 --fatal --autosolve -f "rename" -t "rename2"

if [ -e "a" ]; then
    echo "reversed --autosolve order did not remove the moving source"
    exit 1
fi
if ! printf "same contents\n" | cmp -s - "b"; then
    echo "reversed --autosolve order changed the target contents"
    exit 1
fi

cd ..
mkdir -p "three-claimants/expected"
cd "three-claimants"

for file in a b c; do
    printf "same contents\n" > "expected/$file"
    cp "expected/$file" "$file"
done
printf "a\nb\nc\n" > "rename"
printf "b\nb\nb\n" > "rename2"

expect_failure \
    "--autosolve accepted three claimants for one target" \
    --fatal --autosolve -f "rename" -t "rename2"
check_unchanged "expected" a b c

cd ../..
rm -rf "autosolve-tests"
