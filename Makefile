CC=gcc
CFLAGS=-std=gnu11 -ggdb -O0
OUT=bin/apollo
LDFLAGS=-I../include/ -I../hashmap/include/ -L../hashmap/bin -lhashmap -ldl
FILES=*.c

all:
	cd hashmap; make shared
	cp hashmap/bin/libhashmap.so bin
	cd src; $(CC) $(CFLAGS) $(LDFLAGS) -o ../$(OUT) $(FILES);

debug:
	cd hashmap; make shared
	cp hashmap/bin/libhashmap.so bin
	cd src; $(CC) $(CFLAGS) -DDEBUG $(LDFLAGS) -o ../$(OUT) $(FILES)

plugins:
	cd plugins; make
