#!/bin/sh

set -ex

./build.sh

touch a b c d

cleanup () {
    rm a b c d
}

trap cleanup EXIT

printf "a\nb\nc\nd\na\n" > "/tmp/rename"
printf "b\nc\nd\na\n" > "/tmp/rename2"

bin/brn2 -f "/tmp/rename" -t "/tmp/rename2"
