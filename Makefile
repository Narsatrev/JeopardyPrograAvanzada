ALL= numbers client server preguntas
LDLIBS=-lncurses -lpthread
CFLAGS=-g

ALL:$(ALL)

client:ipc.c client.o
	gcc -g -c ipc.c
	gcc -g -o client ipc.o client.o -lcurses

server:ipc.c server.o
	gcc -g -DSERVER -c ipc.c
	gcc -g -o server ipc.o server.o

clean:
	\rm -rf $(ALL) *.o
