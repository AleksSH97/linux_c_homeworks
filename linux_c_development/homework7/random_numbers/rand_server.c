#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rand_types.h"

int main(int argc, char * argv[]) {
    key_t key;
    int shmid;
    int pid, semid;

    struct sembuf buf[2];
    struct memory_block *mblock;

    /* Generate unique for IPC */
    key = ftok(FTOK_FILE, 1);
    if(key == -1) {
        perror("Failed to generated unique key");
        return EXIT_FAILURE;
    }

    /* Creat semaphore */
    semid = semget(key, 3, 0666 | IPC_CREAT);
    buf[0].sem_num = 0;
    buf[0].sem_flg = SEM_UNDO;
    buf[1].sem_num = 1;
    buf[1].sem_flg = SEM_UNDO;

    semctl(semid, 0, SETVAL, 0);
    shmid = shmget(key, sizeof(struct memory_block), 0666 | IPC_CREAT);

    /* Initialize memory block */
    mblock = (struct memory_block *) shmat(shmid, 0, 0);
    mblock->num = 0;
    memset(mblock->string, 0, sizeof(mblock->string));

    buf[0].sem_op = -1;
    buf[1].sem_op = 1;

    semop(semid, (struct sembuf *)&buf[1], 1);

    while(strcmp("q\n", mblock->string) != 0) {
        semop(semid, (struct sembuf *) &buf, 1);
        printf("Client sends: %d\n\r", mblock->num);
        // sleep(1);
        mblock->num = rand();
        printf("     Server answers with: %d\n\r", mblock->num);
        buf[0].sem_op = -1;
        buf[1].sem_op = 1;
        semop(semid, (struct sembuf*) &buf[1], 1);
    }

    printf("Server exits\n");
    shmdt((void *) mblock);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 2, IPC_RMID);

    return EXIT_SUCCESS;
}
