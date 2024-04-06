PREFIX ?= /usr/local

objs = util.o brn2.o hash.o

.PHONY: all clean install uninstall test
.SUFFIXES:
.SUFFIXES: .c .o

all: release

$(objs): Makefile brn2.h
util.o: util.c util.h
brn2.o: brn2.c brn2.h hash.h
hash.o: hash.c hash.h

CFLAGS += -std=c99 -D_DEFAULT_SOURCE
CFLAGS += -Wextra -Wall
LDFLAGS += -lm

clang: CC=clang
clang: clean release
clang: CFLAGS += -Weverything -Wno-unsafe-buffer-usage -Wno-format-nonliteral

release: CFLAGS += -O2 -flto
release: brn2

debug: CFLAGS += -g -fsanitize=undefined
debug: clean brn2

test: util.c hash.c brn2.c
	./test.sh

brn2: $(objs) main.c
	-ctags --kinds-C=+l *.h *.c
	-vtags.sed tags > .tags.vim
	$(CC) $(CFLAGS) -o $@ $(objs) main.c $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

install: all
	install -Dm755 brn2 $(DESTDIR)$(PREFIX)/bin/brn2
	install -Dm644 brn2.1 $(DESTDIR)$(PREFIX)/man/man1/brn2.1

uninstall: all
	rm -f $(DESTDIR)$(PREFIX)/bin/brn2
	rm -f $(DESTDIR)$(PREFIX)/man/man1/brn2.1

clean:
	rm -rf ./brn2 *.o
