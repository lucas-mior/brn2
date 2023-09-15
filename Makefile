PREFIX ?= /usr/local

objs = util.o main.o hash.o

.PHONY: all clean install uninstall
.SUFFIXES:
.SUFFIXES: .c .o

all: release

$(objs): Makefile brn2.h hash.h
util.o: util.c
main.o: main.c
hash.o: hash.c

CC=cc
ifeq ($(CC),clang)
	CFLAGS += -Weverything -Wno-unsafe-buffer-usage
else
	CFLAGS += -Wextra -Wall
endif

CFLAGS += -std=c99
LDFLAGS += -lm

release: CFLAGS += -O2
release: brn2

debug: CFLAGS += -g -fsanitize=undefined
debug: clean
debug: brn2

brn2: $(objs)
	ctags --kinds-C=+l *.h *.c
	vtags.sed tags > .tags.vim
	$(CC) $(CFLAGS) -o $@ $(objs) $(LDFLAGS)

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
