CC=g++
FLAGS=-std=c++17 -O2 -pedantic -Wall -Wextra -Wno-unused-variable
OUTPUT=solution

all: main.o
	$(CC) $(FLAGS) main.cpp -o $(OUTPUT)
benchmark: benchmark.cpp
	$(CC) $(FLAGS) benchmark.cpp -o benchmark
.cpp.o:
	$(CC) $(FLAGS) -c $< -o $@
clean:
	rm -f *.o $(OUTPUT)
