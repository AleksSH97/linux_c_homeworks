#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define ARRAY_SIZE      (8)
#define ARRAY_HALF_SIZE (ARRAY_SIZE / 2)

static uint array_sum = 0;

static void sum_of_array(int *array) {
    for(uint i = 0; i < ARRAY_HALF_SIZE; i++) {
        array_sum += array[i];
    }
    printf("Sum: %d\n\r", array_sum);
}

static void print_array(int *array) {
    printf("Array: ");
    for(uint i = 0; i < ARRAY_HALF_SIZE; i++) {
        printf("[%d]", array[i]);
        printf(" ");
    }
    printf("\n\r");
}

void * thread_func(void *arg) {
   int i;
   int *array = (int *)arg;

   print_array(array);
   sum_of_array(array);
}

int main(int argc, char ** argv) {
    int array[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};
    int result;
    pthread_t thread1, thread2;

    result = pthread_create(&thread1, NULL, thread_func, array);
    if (result != 0) {
      perror("Creating the first thread");
      return EXIT_FAILURE;
    }

    result = pthread_create(&thread2, NULL, thread_func, (array + ARRAY_HALF_SIZE));
    if (result != 0) {
      perror("Creating the second thread");
      return EXIT_FAILURE;
    }

    result = pthread_join(thread1, NULL);
    if (result != 0) {
      perror("Joining the first thread");
      return EXIT_FAILURE;
    }

    result = pthread_join(thread2, NULL);
    if (result != 0) {
      perror("Joining the second thread");
      return EXIT_FAILURE;
    }

    printf("Done\n");
    return EXIT_SUCCESS;
}
