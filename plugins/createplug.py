import os
import sys

if len(sys.argv) == 2:
    INTERP = "g++ -Wno-write-strings -std=c++11"
else:
    INTERP = "gcc -std=c11"

c = os.system(INTERP+" -fPIC -shared -trigraphs -I../include -I../hashmap/include -L../bin -lhashmap -o ../bin/plugins/plug.so -ggdb -g3 .plug.c 2>&1")

if (c != 0):
    sys.exit(1)
else:
    sys.exit(0)
