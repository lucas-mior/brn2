#!/bin/sh

set -e

./build.sh debug

brn2="$PWD/bin/brn2_debug"

run_brn2_debug () {
    gdb -q -nx -batch --return-child-result \
        -ex run \
        --args "$brn2" "$@"
}

mkdir /tmp/brn2
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
