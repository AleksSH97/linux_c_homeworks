#include <stdio.h>
#include <stdlib.h>

int array_show(const int *array, size_t len) {
    if(array == NULL) {
        return -1;
    }

    if(len <= 0) {
        return -1;
    }

    printf("Array: [");
    for(size_t i = 0; i < len; i++) {
        printf("%d", array[i]);
        if(i < len - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    return 0;
}

int array_fill_with_rand_nums(int *array, size_t len) {
    if(array == NULL) {
        return -1;
    }

    if(len <= 0) {
        return -1;
    }

    for(size_t i = 0; i < len; i++) {
        array[i] = rand();
    }

    return 0;
}
