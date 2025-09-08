#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE (256)
#define FORM_MSG(buffer, len) \
do { \
    int i = 0; \
    char c; \
    while ((i < (BUF_SIZE - 1)) && ((c = getchar()) != '\n')) { \
        buffer[i] = c; \
        i++; \
    } \
    buffer[i] = '\n';  /* Keep the newline */ \
    buffer[i + 1] = '\0'; \
    len = i + 1; \
} while(0)

int main(int argc, char ** argv) {
    uint32_t clen;
    int sock, newsock, port, msgsize;
    char buf[BUF_SIZE];

    struct sockaddr_in serv_addr, cli_addr;

    if(argc < 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0) {
        printf("socket() failed: %d\n", errno);
        return EXIT_FAILURE;
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    port = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("bind() failed: %d\n", errno);
        close(sock);
        return EXIT_FAILURE;
    }

    listen(sock, 1);
    clen = sizeof(cli_addr);
    newsock = accept(sock, (struct sockaddr *) &cli_addr, &clen);

    if(newsock < 0) {
        printf("accept() failed: %d\n", errno);
        return EXIT_FAILURE;
    }

    while(1) {
        int bytes_read = read(newsock, buf, BUF_SIZE - 1);
        if(bytes_read <= 0) {
            printf("Client disconnected or read error\n");
            break;
        }

        buf[bytes_read] = '\0';
        printf("MSG from client: %s", buf);

        if(strncmp(buf, "quit", 4) == 0) {
            break;
        }

        printf("My MSG: ");

        FORM_MSG(buf, msgsize);
        write(newsock, buf, msgsize);
    }

    close(newsock);
    close(sock);
}
