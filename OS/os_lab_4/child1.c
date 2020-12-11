#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

#define MEMORY_NAME "lab4_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 2048
#define BUFFER_SIZE	256

typedef struct buf_sum {
    int buf[BUFFER_SIZE];
    size_t size;
}buf_sum;

void sum_lines(FILE *input, buf_sum *addr){
    char c;
	char buf[BUFFER_SIZE];
	int count_digit = 0;
	int number = 0;
	int result = 0;
	int f_not_left_minus = 0;
	int f_ch = 1;
    addr->size = 0;

	memset(buf, '\0', BUFFER_SIZE);

	while (true) {
        c = fgetc(input);
		if (c == EOF){
			return;
		}
		if (isdigit(c) || c == '-') {
			if (c == '-' && count_digit  > 0){	
				f_not_left_minus = 1;
                perror("incorrect input\n");
                exit(-1);
			}
			buf[count_digit] = c;
			count_digit++;
		}
		if (c == ' ' || c == '\n') {
			
				if (count_digit > 0 || count_digit >= BUFFER_SIZE) {
					number = atoi(buf);
					result += number;
					memset(buf, '\0', count_digit);
					count_digit = 0;
				}
		}
		if(c == '\n') {
            addr->buf[addr->size++] = result;
			result = 0;
		}
	}
}

int main(int argc, const char* argv[]) {
    if(BUFFER_SIZE > SHARED_MEMORY_OBJECT_SIZE){
        perror("Segmentation fault");
    }
    int fd = shm_open(MEMORY_NAME, O_EXCL | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1){ 
        char message[] = "Can't open shared memory file\n";
        fprintf(stderr, "%s", message);
        shm_unlink(MEMORY_NAME);
        close(fd);
        return 1;
    }
    if(ftruncate(fd, SHARED_MEMORY_OBJECT_SIZE) == -1){
        char message[] = "Can't extent shared memory file\n";
        fprintf(stderr, "%s", message);
        shm_unlink(MEMORY_NAME);
        close(fd);
        return 1;
    }

    buf_sum *addr = mmap(NULL, SHARED_MEMORY_OBJECT_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == (void *)-1){
        char message[] = "Mmap error\n";
        fprintf(stderr, "%s", message);
        munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
        shm_unlink(MEMORY_NAME);
        close(fd);
        return 1;
    }
    FILE *input = NULL;
    if (argc == 2) {
        input = fopen(argv[1], "r");
    }
    if(input == NULL){
        char message[] = "Can't open file\n";
        fprintf(stderr, "%s", message);
        munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
        shm_unlink(MEMORY_NAME);
        close(fd);
        return 1;
    }
    sum_lines(input, addr);
    fclose(input);
    munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
    close(fd);
return 0;
}
