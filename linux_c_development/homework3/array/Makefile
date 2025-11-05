sort: main.o array.o sort.o
	gcc main.o array.o sort.o -o sort

main.o: main.c
	gcc main.c -c
array.o: array.c
	gcc array.c -c
sort.o: sort.c
	gcc sort.c -c

clean:
	rm -f *.o
	rm -f sort
	rm -f *~
