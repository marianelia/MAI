#include "translation_sort.h"
#include <stddef.h>
#include <stdlib.h>

char* Translation(long x) {
    int size = 20;
    char tmp;
    char* res = (char*)calloc(size, sizeof(char));
    int i = 0;
    if (x == 0) {
        res[0] = '0';
        return res;
    }
    while (x != 0) {
        if (i > size) { //!!!!!
        char* new_res = (char*)calloc(i, sizeof(char));
        res = new_res;
        }
        if (x % 2 == 0) {
            res[i] = '0';
            x /= 2;
        }
        else {
            res[i] = '1';
            x /= 2;
        }
        i++;
    }

    for (int k = 0; k < i / 2; k++) {
        tmp = res[k];
        res[k] = res[i - k - 1];
        res[i - k - 1] = tmp;
    }
    return res;
}

void Swap(int* x, int* y) {
	int tmp = *x;
	*x = *y;
	*y = tmp;
}


void Sort(int* arr, const int size) {
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 1; j < size; ++j) {
			if (arr[j - 1] > arr[j]) {
				Swap(&arr[j - 1], &arr[j]);
			}
		}
	}
}