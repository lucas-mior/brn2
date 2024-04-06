#!/bin/sh

for src in *.c; do
    [ "$src" = "main.c" ] && continue
    [ "$src" != "util.c" ] && continue
    printf "Testing $src...\n"

    flags="$(awk '/flags:/ { $1=$2=""; print $0 }' "$src")"
    gcc -D MAIN=1 $src -o $src.exe $flags \
        || printf "${RED}Failed to compile $src, is main() defined? ${RES}\n"

    ./$src.exe
done
