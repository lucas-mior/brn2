#!/bin/sh

set -e

./build.sh

rm -rf "/tmp/rename" "/tmp/rename2"

for f in a b c d; do
    echo "$f" >  "$f"
    echo "$f" >> "/tmp/rename"
    echo "$f" >> "/tmp/rename2"
done
echo "a" >> "/tmp/rename"

cleanup () {
    rm a b c d
}

trap cleanup EXIT

set -x
bin/brn2 -f "/tmp/rename" -t "/tmp/rename2"
