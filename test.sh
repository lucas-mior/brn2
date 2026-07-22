#!/bin/sh

set -e

./build.sh

run_brn2_debug () {
    gdb -q -nx -batch --return-child-result \
        -ex run \
        --args bin/brn2_debug "$@"
}

rm -rf "/tmp/rename" "/tmp/rename2"

for f in a b c d; do
    echo "$f" >  "$f"
    echo "$f" >> "/tmp/rename"
done
echo "a" >> "/tmp/rename"

for f in b c d a; do
    echo "$f" >> "/tmp/rename2"
done

cleanup () {
    rm -f a b c d bxx
    rm -f "/tmp/rename" "/tmp/rename2"
}

trap cleanup EXIT

set -x
run_brn2_debug -f "/tmp/rename" -t "/tmp/rename2"

check () {
    if ! grep -q "$1" "$2"; then
        echo "file $2 should be $1"
        exit 1
    fi
}
set +x

check a b
check b c
check c d
check d a

rm -rf "/tmp/rename" "/tmp/rename2"

for f in a b c d; do
    echo "$f" >  "$f"
    echo "$f" >> "/tmp/rename"
done
echo "a" >> "/tmp/rename"

for f in c bxx d a; do
    echo "$f" >> "/tmp/rename2"
done

set -x
run_brn2_debug -f "/tmp/rename" -t "/tmp/rename2"
set +x

check a c
check b bxx
check c d
check d a

rm -f a b c d bxx
rm -rf "/tmp/rename" "/tmp/rename2"

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
}' > "/tmp/rename"

for f in b c d a; do
    echo "$f" >> "/tmp/rename2"
done

set -x
run_brn2_debug -q -f "/tmp/rename" -t "/tmp/rename2" 2>/dev/null
set +x

check a b
check b c
check c d
check d a
