#!/bin/sh

set -e

./build.sh debug

brn2="$PWD/bin/brn2_debug"

run_brn2_debug () {
    if command gdb; then
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
run_brn2_debug -f "rename" -t "rename2"

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
run_brn2_debug -f "rename" -t "rename2"
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
run_brn2_debug -q -f "rename" -t "rename2" 2>/dev/null
set +x

check a b
check b c
check c d
check d a

rm -f a b c d bxx
rm -f "rename" "rename2"

rm -rf "autosolve-validation"
mkdir -p "autosolve-validation/expected"
cd "autosolve-validation"

printf "source a\n" > "expected/a"
printf "source b\n" > "expected/b"
cp "expected/a" "a"
cp "expected/b" "b"
printf "a\nb\n" > "rename"
printf "b\nb\n" > "rename2"

# The second target repeats the first target, but it is also the second
# source's unchanged name. Since a and b differ, --autosolve must reject the
# rename list without modifying either source.
set +e
set -x
run_brn2_debug --fatal --autosolve -f "rename" -t "rename2"
status=$?
set +x
set -e

failed=0
if [ "$status" -eq 0 ]; then
    echo "--autosolve accepted unequal files with the same target"
    failed=1
fi
if ! cmp -s "expected/a" "a"; then
    echo "--autosolve modified source a during validation"
    failed=1
fi
if ! cmp -s "expected/b" "b"; then
    echo "--autosolve modified source b during validation"
    failed=1
fi
if [ "$failed" -ne 0 ]; then
    exit 1
fi

cd ..
rm -rf "autosolve-validation"
