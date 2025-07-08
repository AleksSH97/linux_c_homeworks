#include <stdlib.h>

int array_fill_with_rand_nums(int *array, size_t len);
int array_show(const int *array, size_t len);
int sort_array(int *array, size_t len);

#define SIZE_OF_ARRAY(array) (sizeof(array)/sizeof(array[0]))

int main(void) {
    int nums[10] = {0};

    int err = array_fill_with_rand_nums(nums, SIZE_OF_ARRAY(nums));
    if(err < 0) {
        return 1;
    }

    err = array_show(nums, SIZE_OF_ARRAY(nums));
    if(err < 0) {
        return 1;
    }

    err = sort_array(nums, SIZE_OF_ARRAY(nums));
    if(err < 0) {
        return 1;
    }

    err = array_show(nums, SIZE_OF_ARRAY(nums));
    if(err < 0) {
        return 1;
    }

    return 0;
}
