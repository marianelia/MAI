#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char* argv[]){
	int fd[2];
	int file;
	char file_name[50];
	
	if (argc == 1) {
		char buff[] = "Enter file name: ";
		write(STDOUT_FILENO, &buff, sizeof(buff) - 1);
		read(STDIN_FILENO, &file_name, sizeof(file_name));
		int i;
		for (i = 0; i < 50; i++) {
		    if (file_name[i] == '\n' || file_name[i] == '\0') {
		  		break;
			  }
		}
	  if(file_name[i] == '\n') {
       file_name[i] = '\0';
    }
	} else {
		strcpy(file_name, argv[1]);
	}

    if (pipe(fd) < 0) {
        char message[] = "pipe error\n";
        write(STDERR_FILENO, &message, sizeof(message) - 1);
        return 1;
    }
	
	pid_t pid;
  
	switch(pid=fork()) {
 	case -1 : //ошибка при вызове fork()
		;char message[] = "fork error\n";
        write(STDERR_FILENO, &message, sizeof(message) - 1);
        return 2;

 	case 0 : //это код потомка
     file = open(file_name, O_RDONLY);
     if (file < 0) {
          char message[] = "can't open file\n";
          write(STDERR_FILENO, &message, sizeof(message) - 1);
          return 3;
     }
		dup2(file, STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    dup2(fd[1], STDERR_FILENO);
    close(fd[0]);
    close(fd[1]);
    close(file);
    if (execl("child", "child", (char *) NULL) == -1) {
         char message[] = "exec error\n";
         write(STDERR_FILENO, &message, sizeof(message) - 1);
         return 4;
		}
 
 	default : //код родительского процесса
	    close(fd[1]);
      waitpid(pid, (int *)NULL, 0);
      int result;
      while (read(fd[0], &result, sizeof(result))) {
            char buff[50];
            sprintf(buff, "%d\n", result);
            write(STDOUT_FILENO, &buff, strlen(buff));
      }
      close(fd[0]);
  }
    return 0;
}
