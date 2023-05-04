CC = clang
CFLAGS = -Wall -Wextra -pedantic -O2
PREFIX ?= /usr/local

objs = util.o main.o

.PHONY: all clean install uninstall
.SUFFIXES:
.SUFFIXES: .c .o

all: brn2

$(objs): Makefile brn2.h
util.o: util.c
main.o: main.c

brn2: $(objs)
	$(CC) -O2 -Weverything $(cflags) $(LDFLAGS) -o $@ $(objs) $(ldlibs)

%: %.c Makefile
	$(CC) $(CFLAGS) $< -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 brn2 $(DESTDIR)$(PREFIX)/bin/brn2

uninstall: all
	rm $(DESTDIR)$(PREFIX)/bin/brn2

clean:
	rm -rf ./brn2 *.o
