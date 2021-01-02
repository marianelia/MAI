#include "translation_sort.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int cmd = 0;
	printf("Usage:\n");
	printf("1 number - translation to 2 (or 3) base\n");
	printf("2 size a1 a2 ... - sort \n");

    while (scanf("%d", &cmd) != EOF) {
        if (cmd == 1) {
			long x;
            if((scanf("%ld", &x)== EOF)||(x<0)){
				printf("Error input\n");
                return 1;
            }
            char* string = Translation(x);
			printf("from 10 base to 2(or 3): ");
            printf("%s\n", string);
            free(string);
        }
		if (cmd == 2) {
            int size;
			if((scanf("%d", &size)<1)||(size<0)){
				printf("Error input\n");
				return 1;
			}
			int* a = malloc(sizeof(int) * size);
			for (size_t i = 0; i < size; ++i) {
				if(scanf("%d", &a[i])<1){
					printf("Error input\n");
					return 1;
				}
			}
			Sort(a, size);
			printf("sorted: ");
			for (size_t i = 0; i < size; ++i) {
				printf("%d ", a[i]);
			}
			printf("\n");
			free(a);
        }
    }
}
