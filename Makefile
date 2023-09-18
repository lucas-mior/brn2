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

CC=cc
ifeq ($(CC),clang)
	CFLAGS += -Weverything -Wno-unsafe-buffer-usage
else
	CFLAGS += -Wextra -Wall
endif

CFLAGS += -std=c99
LDFLAGS += -lm

bear: Makefile
	bear -- make > compile_commands.json 

release: CFLAGS += -O2
release: brn2

debug: CFLAGS += -g -fsanitize=undefined
debug: clean
debug: brn2

test: $(objs) test.c
	$(CC) $(CFLAGS) -o $@ $(objs) test.c -lcmocka $(LDFLAGS)
	./test

brn2: $(objs) main.c
	ctags --kinds-C=+l *.h *.c
	vtags.sed tags > .tags.vim
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
	rm -rf ./brn2 *.o ./test
