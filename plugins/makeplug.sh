for i in `find . -iname '*.c' -not -iname '.*.c' -exec basename {} \;`
do echo [makeplug] $@ -o `echo $i | cut -f1 -d '.'`.so $i
$@ -o `echo $i | cut -f1 -d '.'`.so $i
done
