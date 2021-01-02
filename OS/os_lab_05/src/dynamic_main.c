
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stddef.h>

const char* libName1 = "./libsort1.so";
const char* libName2 = "./libsort2.so";

char* (*Translation)(long x);
void (*Sort)(int*, const int);
char *err;

void *libHandle = NULL;

typedef enum {
    FIRST,
    SECOND,
} realization;

realization rel = FIRST;

void load_realization(){
	const char *name;
    if(rel == FIRST){
        name = libName1;
    } else{
        name = libName2;
    }
    libHandle = dlopen(name, RTLD_LAZY);
    if(!libHandle){
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

	*(void **) (&Sort) = dlsym(libHandle, "Sort");
    *(void **) (&Translation) = dlsym(libHandle, "Translation");

    if((err = dlerror())) {
        fprintf(stderr, "%s\n", err);
        exit(EXIT_FAILURE);
    }
}

void change_realization(){
	dlclose(libHandle);
    if(rel == FIRST){
        rel = SECOND;
    } else {
        rel = FIRST;
    }

    load_realization();
}

int main(){
    rel = FIRST;
    load_realization();

    int cmd = 0;
	printf("Usage:\n");
	printf("0 - change realization\n");
	printf("1 number - translation to 2 (or 3) base\n");
	printf("2 size a1 a2 ... - sort \n");
    while (scanf("%d", &cmd) != EOF){

        if(cmd == 0){
            change_realization();
            printf("Ok\n");
            if(rel == FIRST){
                printf("now 1 realization is running\n");
            } else{
                printf("now 2 realization is running\n");
            }
            continue;
        }
        if(cmd == 1) {
            long x;
            if((scanf("%ld", &x)== EOF)||(x<0)){
				printf("Error input\n");
                return 1;
            }
            printf("Translate to ");
            if(rel == FIRST) {
                printf("2 base: ");
            }else{
                printf("3 base: ");
            }
            char* string = Translation(x);
            printf("%s\n", string);
            free(string);
        }
        else if(cmd == 2){
			int size;
			if((scanf("%d", &size) <1)||(size<0)){
				printf("Incorrent input\n");
				return 1;
			}
			int* arr = malloc(sizeof(int) * size);
			for(int i = 0; i<size; i++){
				
				if(scanf("%d", &arr[i])<1){
					printf("Incorrent input");
					return 1;
				}
			}
			Sort(arr, size);
			printf("Sorted: ");
			for (int i = 0; i < size; ++i) {
				printf("%d ", arr[i]);
				
			}
			printf("\n");
			free(arr);
        }
		else{
			printf("incorrect command\n");
		}
    }
    dlclose(libHandle);
}
