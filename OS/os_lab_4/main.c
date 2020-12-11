#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stddef.h>

#define MEMORY_NAME "lab4_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 2048
#define BUFFER_SIZE	50

typedef struct buf_sum {
    int buf[BUFFER_SIZE];
    size_t size;
}buf_sum;

int main(int argc, const char* argv[]){
	pid_t pid;
	int status;
  
	switch(pid=fork()) {
 	case -1 : //ошибка при вызове fork() 
		;char message[] = "fork error\n";
        	fprintf(stderr, "%s", message);
        	return 2;

 	case 0 : //это код потомка
        execl("child1", "child1", argc == 2 ? argv[1] : (char *)NULL, (char *)NULL); 
        perror("Can't execute child\n");
        exit(1);
 	default : //код родительского процесса
        if (waitpid(pid, &status, 0) == -1){
			char message[] = "Waitpid error\n";
        	fprintf(stderr, "%s", message);
			return 1;
		}
        if (WIFSIGNALED(status)) { //возвращает истинное значение, если дочерний процесс завершился из-за необработанного сигнала.
            fprintf(stderr, "Child process terminated by signal: %d\n", WTERMSIG(status));
            shm_unlink(MEMORY_NAME);
            exit(1);
        }
        int fd = shm_open(MEMORY_NAME, O_RDONLY, S_IRUSR | S_IWUSR);
		if (fd == -1){
			char message[] = "Can't open shared memory file\n";
        	fprintf(stderr, "%s", message);
        	shm_unlink(MEMORY_NAME);
        	close(fd);
        	return 1;
		}
	//доступ к памяти, где лежит массив сумм строк
        buf_sum *addr = mmap(NULL, SHARED_MEMORY_OBJECT_SIZE, PROT_READ, MAP_SHARED, fd, 0);
		if(addr == (void *)-1){
			char message[] = "Mmap error\n";
        	fprintf(stderr, "%s", message);
        	munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
        	shm_unlink(MEMORY_NAME);
        	close(fd);
        	return 1;
		}
        for (int i = 0; i < addr->size; i++) {
            printf("%d\n", addr->buf[i]);
        }
        munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
        shm_unlink(MEMORY_NAME);
        close(fd);
  	}
    return 0;
}
