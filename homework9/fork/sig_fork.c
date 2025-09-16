#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void term_handler(int i) {
  printf ("Got signal, terminating\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    int pipedes[2];
    pipe(pipedes);

    pid_t pid = fork();

    if(pid > 0) {
        close(pipedes[0]);

        struct sigaction sa;
        sigset_t newset;
        sigemptyset(&newset);
        sigaddset(&newset, SIGHUP);
        sigprocmask(SIG_BLOCK, &newset, 0);

        sa.sa_handler = term_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGTERM, &sa, NULL);

        pid_t fork_pid = getpid();
        printf("Parent pid is %d\n", fork_pid);

        write(pipedes[1], (int *)&fork_pid, sizeof(fork_pid));
        close(pipedes[1]);

        printf("Waiting...\n");
        while(1) sleep(1);
    } else if(pid == 0) {
        close(pipedes[1]);

        pid_t buf;
        int len = read(pipedes[0], &buf, sizeof(buf));

        if(len > 0) {
            printf("Child received PID: %d, sending SIGTERM\n", buf);
            kill(buf, SIGTERM);
        } else {
            printf("Child failed to read PID\n");
        }

        return EXIT_SUCCESS;
    } else {
        perror("Fork failed");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
