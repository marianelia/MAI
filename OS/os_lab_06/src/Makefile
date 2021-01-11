CC=g++
CFLAGS=-std=c++11 -g -O2 -Wall -Wno-unused-variable

LDFLAGS=-lrt -lzmq
SRC=server.cpp zmq_func.cpp avl.cpp
OBJ=$(SRC:.cpp=.o)

SRC2=client.cpp zmq_func.cpp
OBJ2=$(SRC2:.cpp=.o)

all: server client

server: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

client: $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ2) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

client.o: zmq_func.hpp avl.h
server.o: zmq_func.hpp avl.h
avl.o: avl.h

clean:
	rm -f *.o

