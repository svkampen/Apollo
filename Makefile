CC=gcc
CFLAGS=-std=gnu11 -ggdb -O0
OUT=bin/apollo
LDFLAGS=-I../include/ -L../bin -lhashmap
FILES=*.c

all:
	cd src; $(CC) $(CFLAGS) $(LDFLAGS) -o ../$(OUT) $(FILES)

debug:
	cd src; $(CC) $(CFLAGS) -DDEBUG $(LDFLAGS) -o ../$(OUT) $(FILES)
