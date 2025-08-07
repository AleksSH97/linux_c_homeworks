all: msgdemo
msgdemo:
	gcc msgserver.c -o msgserver
	gcc msgclient.c -o msgclient

clean:
	rm -f *.o
	rm -f *server *client
	rm -f *~
