#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT_TIMES 5

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

void *fn_thread1();
void *fn_thread2();

static volatile int turn = 1;

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
        pthread_mutex_lock(&mtx);

        while(turn != 1) {
            pthread_cond_wait(&cond, &mtx);
        }

        printf("Thread1: Tic\n\r");
        turn = 2;

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);

        count_thread++;
    }

    return NULL;
}

void *fn_thread2()
{
    int count_thread = 0;

    while(count_thread < COUNT_TIMES) {
        pthread_mutex_lock(&mtx);
        while(turn != 2) {
            pthread_cond_wait(&cond, &mtx);
        }

        printf("Thread2: Tac\n\r");
        turn = 1;

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);

        count_thread++;
    }

    return NULL;
}
