#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int array_fill_with_rand_nums(int *array, size_t len);
int array_show(const int *array, size_t len);
int sort_array(int *array, size_t len);

#define NUM_CHARS_IN_INT64   (21) /* 20 characters in int64 number + \0 */
#define SIZE_OF_ARRAY(array) (sizeof(array)/sizeof(array[0]))

int main(int argc, char ** argv) {
    int nums[7] = {0};
    char number[NUM_CHARS_IN_INT64];
    int char_idx, num_index;
    ssize_t ret;
    bool in_number = false;
    char ch;

    if(argc < 2) {
        fprintf(stderr, "Too few arguments\n");
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd < 0) {
        fprintf (stderr, "Cannot open file\n");
        exit (1);
    }

    while((ret = read(fd, &ch, 1)) > 0 && num_index < 10) {
        putchar(ch);

        if (isdigit(ch) || ch == '-') {
            if(!in_number) {
                in_number = true;
                char_idx = 0;
                memset(number, 0, sizeof(number));
            }

            if(char_idx < (NUM_CHARS_IN_INT64 - 1)) {
                number[char_idx++] = ch;
            }
        } else if (ch == ',') {
            number[char_idx] = '\0';
            nums[num_index++] = atoi(number);

            in_number = false;
        }
    }

    /* For last number in file */
    if (in_number && num_index < 10 && char_idx > 0) {
        number[char_idx] = '\0';
        char clean_number[NUM_CHARS_IN_INT64];
        int clean_index = 0;
        for (int i = 0; i < char_idx; i++) {
            if (number[i] != ',') {
                clean_number[clean_index++] = number[i];
            }
        }
        clean_number[clean_index] = '\0';

        nums[num_index++] = atoi(clean_number);
    }

    close(fd);

    int err = array_show(nums, SIZE_OF_ARRAY(nums));
    if(err < 0) {
        exit(1);
    }

    err = sort_array(nums, SIZE_OF_ARRAY(nums));
    if(err < 0) {
        exit(1);
    }

    err = array_show(nums, SIZE_OF_ARRAY(nums));
    if(err < 0) {
        exit(1);
    }

    return 0;
}
