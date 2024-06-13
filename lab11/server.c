#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

#include "common.h"

int client_sockfd[MAX_USERS];
char client_name[MAX_USERS][USERNAME_LEN];

#define SHARED_MEM "/lab11"

volatile bool running = true;
void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("Received SIGINT\n");
        running = false;
        for (int i = 0; i < MAX_USERS; i++)
        {
            if (client_sockfd[i] > 0)
            {
                Message msg;
                msg.type = STOP;
                send(client_sockfd[i], &msg, sizeof(msg), 0);
                close(client_sockfd[i]);
            }
        }
        printf("Server closed\n");
        exit(0);
    }
}

typedef struct
{
    int i;
    int sockfd;
} args_t;

void *client_handler(void *arg)
{
    args_t args = *(args_t *)arg;
    int i = args.i;
    int sockfd = args.sockfd;
    time_t prev_time = time(NULL);
    while (running)
    {
        Message msg;
        if (recv(client_sockfd[i], &msg, sizeof(msg), MSG_DONTWAIT) > 0)
        {
            if (msg.type == ALIVE)
            {
                printf("ALIVE %s\n", msg.from_user);
                strcpy(client_name[i], msg.from_user);
            }
            else if (msg.type == STOP)
            {
                printf("STOP %s\n", client_name[i]);
                close(client_sockfd[i]);
                client_sockfd[i] = 0;
            }
            else if (msg.type == TOALL)
            {
                printf("TOALL %s:\n %s\n", client_name[i], msg.data.toall);
                for (int j = 0; j < MAX_USERS; j++)
                {
                    if (client_sockfd[j] > 0 && j != i)
                    {
                        send(client_sockfd[j], &msg, sizeof(msg), 0);
                    }
                }
            }
            else if (msg.type == TOONE)
            {
                printf("TOONE %s -> %s:\n %s\n", client_name[i], msg.data.toone.to_user, msg.data.toone.message);
                for (int j = 0; j < MAX_USERS; j++)
                {
                    if (client_sockfd[j] > 0 && strcmp(client_name[j], msg.data.toone.to_user) == 0)
                    {
                        if (send((client_sockfd[j]), &msg, sizeof(msg), 0) < 0)
                        {
                            printf("User %s is not connected\n", msg.data.toone.to_user);
                        }
                        break;
                    }
                }
            }
            else if (msg.type == LIST)
            {
                printf("LIST %s\n", client_name[i]);
                Message response;
                response.type = LIST;
                response.data.list.user_count = 0;
                for (int j = 0; j < MAX_USERS; j++)
                {
                    if (client_sockfd[j] > 0)
                    {
                        strcpy(response.data.list.user_list[response.data.list.user_count++], client_name[j]);
                    }
                }
                send(client_sockfd[i], &response, sizeof(response), 0);
            }
        } // end recv

        // ping user
        time_t cur_time = time(NULL);
        if (cur_time - prev_time > 5)
        {
            Message msg;
            msg.type = ALIVE;
            strcpy(msg.data.list.user_list[0], client_name[i]);
            send(client_sockfd[i], &msg, sizeof(msg), 0);
            printf("check if alive %s\n", client_name[i]);
            prev_time = cur_time;
            if (recv(client_sockfd[i], &msg, sizeof(msg), 0) <= 0)
            {
                printf("client %s is dead\n", client_name[i]);
                close(client_sockfd[i]);
                client_sockfd[i] = 0;
                return NULL;
            }
        }
    }
}

// argv[1] = server_ip
// argv[2] = server_port
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }
    signal(SIGINT, sig_handler);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    int shared_memory_fd = shm_open(SHARED_MEM, O_CREAT | O_RDWR, 0666);
    ftruncate(shared_memory_fd, sizeof(server_data_t));
    server_data_t *server_data = mmap(NULL, sizeof(server_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        return 1;
    }
    if (listen(sockfd, MAX_USERS) < 0)
    {
        perror("listen");
        return 1;
    }

    while (running)
    {
        // accept new clients
        int clientfd, i = 0;
        if ((clientfd = accept(sockfd, NULL, NULL)) > 0)
        {
            for (; i < MAX_USERS; i++)
            {
                if (client_sockfd[i] == 0)
                {
                    client_sockfd[i] = clientfd;
                    Message msg;
                    msg.type = ALIVE;
                    send(clientfd, &msg, sizeof(msg), 0);
                    printf("Client %d connected:\n", i);
                    break;
                }
            }
        }

        // if server is full, close the client socket
        if (i == MAX_USERS)
        {
            printf("Server is full\n");
            Message msg;
            msg.type = STOP;
            strcpy(msg.from_user, "Server");
            strcpy(msg.data.toall, "Server is full");
            send(clientfd, &msg, sizeof(msg), 0);
            close(clientfd);
        }

        // receive messages from clients
        pthread_t client_thr;
        args_t args = {i, clientfd};
        pthread_create(&client_thr, NULL, client_handler, &args);
    }
    return 0;
}
