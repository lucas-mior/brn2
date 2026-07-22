#!/bin/sh

set -e

./build.sh

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
    rm a b c d bxx
}

trap cleanup EXIT

set -x
bin/brn2 -f "/tmp/rename" -t "/tmp/rename2"

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
bin/brn2 -f "/tmp/rename" -t "/tmp/rename2"

check a c
check b bxx
check c d
check d a
