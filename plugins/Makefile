CC=gcc
CFLAGS=-fPIC -shared -I../include -I../hashmap/include -ggdb -g3
LDFLAGS=-L../bin -lhashmap -ldl

all:
	bash makeplug.sh $(CC) $(CFLAGS) $(LDFLAGS)
	mv *.so ../bin/plugins/
