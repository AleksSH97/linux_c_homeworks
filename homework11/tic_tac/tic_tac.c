#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT_TIMES 5

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;

void *fn_thread1();
void *fn_thread2();

int main(int argc, char **argv)
{
    pthread_t thread1, thread2;

    pthread_create( &thread1, NULL, &fn_thread1, NULL);
    pthread_create( &thread2, NULL, &fn_thread2, NULL);

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);

    exit(0);
}

void *fn_thread1()
{
    int count_thread = 0;

    while(count_thread < COUNT_TIMES) {
        printf("Thread1: Tic\n\r");
        sleep(1);

        pthread_cond_signal(&condition_cond);

        pthread_mutex_lock(&condition_mutex);
        pthread_cond_wait(&condition_cond, &condition_mutex);
        pthread_mutex_unlock(&condition_mutex);

        count_thread++;
    }

    pthread_cond_signal(&condition_cond);
    printf("Thread1: BREAK\n\r");
    return NULL;
}

void *fn_thread2()
{
    int count_thread = 0;

    while(count_thread < COUNT_TIMES) {
        pthread_mutex_lock(&condition_mutex);
        pthread_cond_wait(&condition_cond, &condition_mutex);
        pthread_mutex_unlock(&condition_mutex);

        printf("Thread2: Tac\n\r");
        sleep(1);

        pthread_cond_signal(&condition_cond);

        count_thread++;
    }

    pthread_cond_signal(&condition_cond);
    printf("Thread2: BREAK\n\r");
    return NULL;
}
