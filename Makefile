CC=gcc
CFLAGS=-std=gnu11 -O2 -ggdb
OUT=bin/apollo
LDFLAGS=
FILES=*.c

all:
	cd src; $(CC) $(CFLAGS) $(LDFLAGS) -o ../$(OUT) $(FILES)
