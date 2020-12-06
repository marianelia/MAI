#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_true;

int get_rand_range_int(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

void* tread_count(void* arg){
    int num1 = get_rand_range_int(1,13);
    int num2 = get_rand_range_int(1,13);
    int suit1 = get_rand_range_int(1,4);
    int suit2 = get_rand_range_int(1,4);
    if(num1 == num2 && suit1 != suit2){
        count_true++;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int number_of_treads = 0;
    char string_number_of_treads[10];
	if (argc == 1) {
		printf("Enter number of treads: \n");
		scanf("%d",&number_of_treads);		
	} else {
		strcpy(string_number_of_treads, argv[1]);
        number_of_treads = atoi(string_number_of_treads);
	}

    pthread_t threads[number_of_treads];
    count_true = 0;
    int status;
    int status_addr;
    for(int i =0; i<number_of_treads; ++i){
        status = pthread_create(&threads[i], NULL, tread_count, (void *)i);
        if (status != 0) {
            printf("main error: can't create thread, status = %d\n", status);
            exit(-1);
        }
    }
    for(int i =0; i<number_of_treads; ++i){
        status = pthread_join(threads[i],NULL);
       if (status != 0) {
            printf("main error: can't join thread, status = %d\n", status);
            exit(-2);
        }
    }
    double probability = (double) count_true / number_of_treads;
    printf("%d\n", count_true);
    printf("%d\n", number_of_treads);
     printf("%f\n", probability);
     return 0;

}
