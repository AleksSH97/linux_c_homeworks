#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "rand_types.h"

#define DEFAULT_TEXT "\033[0m"
#define GREEN_TEXT     "\033[1;32m"

int main(int argc, char * argv[]) {
    key_t key;
    int shmid;
    int semid;

    struct sembuf buf[2];
    struct memory_block *mblock;

    /* Generate unique for IPC */
    key = ftok(FTOK_FILE, 1);
    if(key == -1) {
        perror("Failed to generated unique key");
        return EXIT_FAILURE;
    }

    /* Update randomizer with new seed */
    srand((unsigned int)time(NULL));

    /* Creat semaphore */
    semid = semget(key, 2, 0666);
    buf[0].sem_num = 0;
    buf[0].sem_flg = SEM_UNDO;
    buf[1].sem_num = 1;
    buf[1].sem_flg = SEM_UNDO;

    semctl(semid, 0, SETVAL, 0);
    shmid = shmget(key, sizeof(struct memory_block), 0666 | IPC_CREAT);

    /* Initialize memory block */
    mblock = (struct memory_block *) shmat(shmid, 0, 0);
    mblock->num = rand();
    memset(mblock->string, 0, sizeof(mblock->string));

    buf[1].sem_op = -1;

    printf("Exit: write letter " GREEN_TEXT "q" DEFAULT_TEXT "\n");
    printf("Continue: press " GREEN_TEXT "ENTER" DEFAULT_TEXT "\n\n");

    while (strcmp("q\n", mblock->string) != 0) {
      int i = 0;
      semop(semid, (struct sembuf*) &buf[1], 1);
      printf("Server sends %d\n", mblock->num);

      while ((i < (MAXLEN - 1)) && ((mblock->string[i++] = getchar()) !=  '\n') );
      mblock->string[i] = 0;

      mblock->num = rand();
      printf("     Client answers with: %d\n\r", mblock->num);
      buf[0].sem_op = 1;
      buf[1].sem_op = -1;
      semop(semid, (struct sembuf*) &buf, 1);
    }

    printf("Client exits\n");
    shmdt((void *) mblock);

    return EXIT_SUCCESS;
}
