CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ggdb
PREFIX ?= /usr/local

.PHONY: all clean install

all: brn2

%: %.c
	$(CC) $(CFLAGS) $< -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 brn2 $(DESTDIR)$(PREFIX)/bin/brn2

clean:
	rm -rf ./brn2
