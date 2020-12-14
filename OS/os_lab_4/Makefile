CC=gcc
CFLAGS=-std=gnu99 -g -O2 -pedantic -Wall -Wno-unused-variable
LDFLAGS=-lrt
SRC=parent.c
OBJ=$(SRC:.c=.o)

SRC2=child.c
OBJ2=$(SRC2:.c=.o)

all: prog child

prog: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

child: $(OBJ2)
	$(CC) $(CFLAGS) $(OBJ2) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
