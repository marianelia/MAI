FLAGS = -g -O2 -pedantic -std=c11 -Wall

#export LD_LIBRARY_PATH=/home/maria/project/os5_tmp/:$LD_LIBRARY_PATH

all: static1 static2 dynamic

static1: static_main libsort1.so
	gcc $(FLAGS) static_main.o  -L. -lsort1 -o solution_s1  -Wl,-rpath,.

static2: static_main libsort2.so
	gcc $(FLAGS) static_main.o  -L. -lsort2 -o solution_s2  -Wl,-rpath,.


static_main: static_main.c
	gcc $(FLAGS) -c static_main.c

dynamic: dynamic_main dynamic1 dynamic2
	gcc $(FLAGS) dynamic_main.o -o solution -ldl 

dynamic_main: dynamic_main.c
	gcc $(FLAGS) -c dynamic_main.c -ldl 

dynamic1: translation_sort1.o
	gcc $(FLAGS) -shared -o libsort1.so translation_sort1.o

dynamic2: translation_sort2.o
	gcc $(FLAGS) -shared -o libsort2.so translation_sort2.o

translation_sort1.o: translation_sort.h translation_sort1.c
	gcc $(FLAGS) -c -fPIC translation_sort1.c

translation_sort2.o: translation_sort.h translation_sort2.c
	gcc $(FLAGS) -c -fPIC translation_sort2.c

clean:
	rm -rf *.o *.so *.a solution*