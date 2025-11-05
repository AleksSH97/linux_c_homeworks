#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#include "msgtypes.h"

static int wait_for_msg(int msgid, struct msg_t *msg) {
    if(msg == NULL) {
        return -EINVAL;
    }

    while(1) {
        if(msgrcv(msgid, msg, sizeof(struct msg_t) - sizeof(msg->mtype), 0, 0) == -1) {
            perror("Message receive on server side");
            return -1;
        }

        printf("Client (pid = %i) sent: %s\n", msg->snd_pid, msg->body);

        if(strcmp(msg->body, "quit") == 0) {
            break;
        }

        char buffer[MAXLEN] = {0};
        printf("Server response: ");
        fflush(stdout);

        SEND_MSG(buffer);

        struct msg_t reply;
        reply.mtype = 1;
        reply.snd_pid = getpid();
        strcpy(reply.body, buffer);

        if(msgsnd(msgid, &reply, sizeof(struct msg_t) - sizeof(reply.mtype), 0) == -1) {
            perror("Message send on server side");
            return -1;
        }
    }

    return 0;
}

int main(int argc, char * argv[]) {
    struct msg_t message;
    int msgid;
    char * response = "Ok!";

    printf("Starting server...\n");

    msgid = msgget(KEY, 0666 | IPC_CREAT);

    printf("Server ready. Waiting for client messages...\n");

    int err = wait_for_msg(msgid, &message);
    if(err < 0) {
        printf("ERROR: waiting for message from client: %d\n", err);
        msgctl(msgid, IPC_RMID, 0);
        return EXIT_FAILURE;
    }

    msgctl(msgid, IPC_RMID, 0);

    return EXIT_SUCCESS;
}
