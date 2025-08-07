
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#include "msgtypes.h"

static int chat_with_server(int msgid) {
    struct msg_t message, reply;
    char buffer[MAXLEN] = {0};

    while(1) {
        printf("Client message (type 'quit' to exit): ");
        fflush(stdout);

        SEND_MSG(buffer);

        message.mtype = 1;
        message.snd_pid = getpid();
        strcpy(message.body, buffer);

        if(msgsnd(msgid, &message, sizeof(struct msg_t) - sizeof(message.mtype), 0) == -1) {
            perror("Message send on client side");
            return -1;
        }

        if(strcmp(buffer, "quit") == 0) {
            break;
        }

        if(msgrcv(msgid, &reply, sizeof(struct msg_t) - sizeof(reply.mtype), 0, 0) == -1) {
            perror("Message receive on client side");
            return -1;
        }

        printf("Server (pid = %i) sent: %s\n", reply.snd_pid, reply.body);
    }

    return 0;
}

int main(int argc, char * argv[]) {
    int msgid;
    int i;

    printf("Starting client...\n");

    msgid = msgget(KEY, 0666);
    if(msgid == -1) {
       printf("ERROR: Server is not running! ID: %d\n", msgid);
       return EXIT_FAILURE;
    }

    printf("Connected to server. Starting chat...\n");

    if(chat_with_server(msgid) < 0) {
        printf("ERROR: Communication with server failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
