TARGETS = echo_client echo_server
OBJECTS = csapp.o echo_client.o echo_server.o

all : $(TARGETS)

echo_client : csapp.o echo_client.o
	gcc csapp.o echo_client.o -lpthread -o echo_client

echo_server : csapp.o echo_server.o
	gcc csapp.o echo_server.o -lpthread -o echo_server

csapp.o : csapp.h csapp.c
	gcc -c -lpthread csapp.c

echo_client.o : csapp.h echo_client.c
	gcc -c echo_client.c

echo_server.o : csapp.h echo_server.c
	gcc -c echo_server.c

.PHONY : clean
clean :
	-rm $(TARGET) $(OBJECTS)
