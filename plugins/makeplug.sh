for i in `find . -iname '*.c' -exec basename {} \;`
do $@ -o `echo $i | cut -f1 -d '.'`.so
done
