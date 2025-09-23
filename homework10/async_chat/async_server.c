#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

#define MAX_NUM_CLIENTS (5)
#define BUF_SIZE        (256)

static volatile uint8_t clients_num = 0;
static volatile int sockets[MAX_NUM_CLIENTS];

typedef struct {
    int socket;
    int client_id;
} client_data_t;

static int conf_server(struct sockaddr_in *serv_addr, int port, int sock) {
    memset((char *) serv_addr, 0, sizeof(struct sockaddr_in));

    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = INADDR_ANY;
    serv_addr->sin_port = htons(port);

    if(bind(sock, (struct sockaddr *) serv_addr, sizeof(struct sockaddr_in)) < 0) {
        printf("bind() failed: %d\n", errno);
        close(sock);
        return -1;
    }

    return 0;
}

void * thread_func(void *arg) {
    client_data_t *data = (client_data_t *)arg;
    int socket = data->socket;
    int client_num = data->client_id;
    char buf[BUF_SIZE];

    sockets[client_num] = socket;

    printf("Client #%d have joined!\n", client_num);
    printf("Socket: %d\n", socket);

    while(1) {
        int bytes_read = read(socket, buf, BUF_SIZE - 1);
        if(bytes_read <= 0) {
            printf("Client disconnected or read error\n");
            break;
        }

        buf[bytes_read] = '\0';

        if(strncmp(buf, "quit", 4) == 0) {
            break;
        }

        for(uint8_t i = 0; i < clients_num; i++) {
            if(sockets[i] > 0 && sockets[i] != socket) {
                write(sockets[i], buf, bytes_read);
            }
        }
    }

    printf("Client %d left chat!\n", client_num);
    close(socket);
    sockets[client_num] = -1;
    free(data);
    return NULL;
}

int main(int argc, char **argv) {
    uint32_t clen;
    int sock, newsock, port, msgsize;
    char buf[BUF_SIZE];
    pthread_t threads[MAX_NUM_CLIENTS];

    struct sockaddr_in serv_addr, cli_addr;

    if(argc < 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        printf("Failed to create a new socket: %d\n", errno);
        return EXIT_FAILURE;
    }

    port = atoi(argv[1]);
    int err = conf_server(&serv_addr, port, sock);
    if(err != 0) {
        printf("Failed to configurate server: %d\n", err);
        return EXIT_FAILURE;
    }

    printf("Starting to listen for clients on port %d...\n", port);

    err = listen(sock, MAX_NUM_CLIENTS);
    if(err != 0) {
        printf("Failed to prepare to accept connections: %d\n", errno);
        return EXIT_FAILURE;
    }

    while(1) {
        if(clients_num >= MAX_NUM_CLIENTS) {
             printf("Max number of clients reached!\n");
             sleep(1);
             continue;
        }

        clen = sizeof(cli_addr);
        newsock = accept(sock, (struct sockaddr *) &cli_addr, &clen);
        if(newsock < 0) {
            printf("accept() failed: %d\n", errno);
            continue;  // Continue accepting other clients
        }

        client_data_t *client_data = malloc(sizeof(client_data_t));
        if(!client_data) {
            printf("Failed to allocate memory for client data\n");
            close(newsock);
            continue;
        }

        client_data->socket = newsock;
        client_data->client_id = clients_num;

        err = pthread_create(&threads[client_data->client_id], NULL, thread_func, client_data);
        if (err != 0) {
            perror("Creating thread failed");
            close(newsock);
            free(client_data);
            clients_num--;
            continue;
        }

        pthread_detach(threads[client_data->client_id]);

        clients_num++;
    }

    close(sock);
    return EXIT_SUCCESS;
}
