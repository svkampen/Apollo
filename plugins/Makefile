CC=gcc
CFLAGS=-fPIC -shared -I../include -I../hashmap/include
LDFLAGS=-L../bin -lhashmap

all:
	bash makeplug.sh $(CC) $(CFLAGS) $(LDFLAGS)
	mv *.so ../bin/plugins/
