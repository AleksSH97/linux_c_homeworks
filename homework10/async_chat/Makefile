async_server: async_server.o
	gcc async_server.o -o async_server

async_server.o: async_server.c
	gcc async_server.c -c

clean:
	rm -f *.o
	rm -f async_server
	rm -f *~
