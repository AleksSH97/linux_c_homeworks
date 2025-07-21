sort: main.o libsort.so
	gcc -o sort main.o -L. -lsort -Wl,-rpath,.

main.o: main.c
	gcc main.c -c

libsort.so: array.o sort.o
	gcc -shared -o libsort.so array.o sort.o

array.o: array.c
	gcc array.c -c -fPIC

sort.o: sort.c
	gcc sort.c -c -fPIC
