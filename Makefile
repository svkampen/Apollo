CC=gcc
CFLAGS=-std=gnu11 -ggdb -O2 -Wl,--dynamic-list=../export -Wall -Werror
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

plugs:
	mkdir -p bin/plugins
	cd hashmap; make shared
	cp hashmap/bin/libhashmap.so bin
	cd plugins; make all
