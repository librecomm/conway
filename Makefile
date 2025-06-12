CC=cc
CFLAGS=-std=c99 -Wall -Werror

PREFIX=/usr/local

build: conway.c
	$(CC) $(CFLAGS) -o conway conway.c

install: build
	mv ./conway $(PREFIX)/bin/

uninstall: build
	rm -f $(PREFIX)/bin/conway

clean:
	rm -f ./conway
