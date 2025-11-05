#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if(argc == 1) {
        printf("Program received only one argument\n");
        printf("Pass program PID as argument\n\r");
        return EXIT_FAILURE;
    }

    if (argv[1] == NULL) {
        return EXIT_FAILURE;
    }

    int pid = atoi(argv[1]);
    printf("Signaling to %d PID process\n", pid);

    kill(pid, SIGTERM);

    return EXIT_SUCCESS;
}
