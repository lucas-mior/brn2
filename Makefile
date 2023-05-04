CC = clang
PREFIX ?= /usr/local

objs = util.o main.o hash.o

.PHONY: all clean install uninstall
.SUFFIXES:
.SUFFIXES: .c .o

all: release

$(objs): Makefile brn2.h
util.o: util.c
main.o: main.c
hash.o: hash.c

release: cflags += -O2 -Weverything
release: stripflag = -s
release: brn2

debug: cflags += -g -Weverything
debug: clean
debug: brn2

brn2: $(objs)
	$(CC) $(stripflag) $(cflags) $(LDFLAGS) -o $@ $(objs) $(ldlibs)

%: %.c Makefile
	$(CC) $(stripflag) $(cflags) $(CFLAGS) $< -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 brn2 $(DESTDIR)$(PREFIX)/bin/brn2

uninstall: all
	rm $(DESTDIR)$(PREFIX)/bin/brn2

clean:
	rm -rf ./brn2 *.o
