#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT_DONE  10
#define COUNT_HALT1  2
#define COUNT_HALT2  5

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;

static int count = 0;

void *functionCount1();
void *functionCount2();

int main(int argc, char **argv)
{
   pthread_t thread1, thread2;

   count = 0;

   pthread_create( &thread1, NULL, &functionCount1, NULL);
   pthread_create( &thread2, NULL, &functionCount2, NULL);
   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   exit(0);
}

/**
 *  HOMEWORK:
 *  COUNT1: 0, 1, 2             7, 8, 9, |10, 11|
 *  COUNT2:         3, 4, 5, 6,          |10, 11|
 */

 void *functionCount1()
 {
     printf("Counter value functionCount1: %d\n", count);

     for(;;) {
         pthread_mutex_lock( &count_mutex );
         printf("Counter value functionCount1: %d\n", ++count);
         pthread_mutex_unlock( &count_mutex );

         pthread_mutex_lock( &condition_mutex );
         if(count >= COUNT_HALT1 && count <= COUNT_HALT2) {
             pthread_cond_wait( &condition_cond, &condition_mutex );
         }
         pthread_mutex_unlock( &condition_mutex );

         if(count >= COUNT_DONE) return(NULL);
     }
 }

 void *functionCount2()
 {
     for(;;)
     {
         if( count >= COUNT_HALT1 && count <= COUNT_HALT2 ) {
             pthread_mutex_lock( &count_mutex );
             count++;
             printf("Counter value functionCount2: %d\n",count);
             pthread_mutex_unlock( &count_mutex );
         }

         pthread_mutex_lock( &condition_mutex );
         if( count < COUNT_HALT1 || count > COUNT_HALT2 ) {
             pthread_cond_signal( &condition_cond );
         }
         pthread_mutex_unlock( &condition_mutex );

         if(count >= COUNT_DONE) return(NULL);
     }
 }
