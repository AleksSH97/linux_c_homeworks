#include <stdlib.h>
#include <stdio.h>

int sort_array(int *array, size_t len) {
    if(array == NULL) {
        return -1;
    }

    if(len == 0) {
        return -1;
    }

    printf("Started sorting array\n");

    for(size_t i = 0; i < len - 1; i++) {
        size_t min_index = i;

        for(size_t j = i + 1; j < len; j++) {
            if(array[j] < array[min_index]) {
                min_index = j;
            }
        }

        if(min_index != i) {
            int temp = array[i];
            array[i] = array[min_index];
            array[min_index] = temp;
        }
    }

    printf("Finished sorting array\n");

    return 0;
}
