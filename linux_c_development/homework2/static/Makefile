sort: main.o libsort.a
	gcc -o sort main.o -L. -lsort

main.o: main.c
	gcc main.c -c

libsort.a: array.o sort.o
	ar cr libsort.a array.o sort.o

array.o: array.c
	gcc array.c -c
sort.o: sort.c
	gcc sort.c -c
