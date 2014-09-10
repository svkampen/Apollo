import os
import sys

c = os.system("gcc -fPIC -shared -I../include -I../hashmap/include -L../bin -lhashmap -o ../bin/plugins/plug.so -ggdb -g3 .plug.c 2>&1")

if (c != 0):
    sys.exit(1)
else:
    sys.exit(0)
