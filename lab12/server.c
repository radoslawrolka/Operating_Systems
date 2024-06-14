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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>

#include "common.h"

struct sockaddr_in clients[MAX_USERS];
char client_name[MAX_USERS][USERNAME_LEN];

volatile bool running = true;
void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("Received SIGINT\n");
        running = false;
    }
}


// argv[1] = server_ip
// argv[2] = server_port
int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }
    signal(SIGINT, sig_handler);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        return 1;
    }

    struct sockaddr_in client_addr;
    pid_t pid;
    if ((pid = fork())==0) {
        while (running) {
            Message msg;
            socklen_t client_addr_len = sizeof(client_addr);
            if (recvfrom(sockfd, &msg, sizeof(Message), 0, (struct sockaddr *)&client_addr, &client_addr_len) < 0) {
                perror("recvfrom");
                return 1;
            }
            
            if (msg.type == TOALL) {
                printf("TOALL %s:\n %s\n", msg.from_user, msg.message);
                for (int i = 0; i < MAX_USERS; i++) {
                    if (clients[i].sin_port > 0) {
                        sendto(sockfd, &msg, sizeof(Message), 0, (struct sockaddr *)&clients[i], sizeof(clients[i]));
                    }
                }
            } else if (msg.type == TOONE) {
                printf("TOONE %s -> %s:\n %s\n", msg.from_user, msg.to_user, msg.message);
                int i = 0;
                for (; i < MAX_USERS; i++) {
                    if (clients[i].sin_port > 0 && strcmp(client_name[i], msg.to_user) == 0) {
                        sendto(sockfd, &msg, sizeof(Message), 0, (struct sockaddr *)&clients[i], sizeof(clients[i]));
                        break;
                    }
                }
            } else if (msg.type == STOP) {
                printf("STOP %s\n", msg.from_user);
                for (int i = 0; i < MAX_USERS; i++) {
                    if (clients[i].sin_port > 0 && strcmp(client_name[i], msg.from_user) == 0) {
                        clients[i].sin_port = 0;
                        break;
                    }
                }
            } else if (msg.type == ALIVE) {
                printf("ALIVE %s\n", msg.from_user);
                bool found = false;
                for (int i = 0; i < MAX_USERS; i++) {
                    if (clients[i].sin_port == 0) {
                        clients[i] = client_addr;
                        strcpy(client_name[i], msg.from_user);
                        sendto(sockfd, &msg, sizeof(Message), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    printf("No more space for new clients\n");
                    msg.type = STOP;
                    sendto(sockfd, &msg, sizeof(Message), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                }
            } else if (msg.type == LIST) {
                printf("LIST %s\n", msg.from_user);
                msg.type = LIST;
                msg.user_count = 0;
                for (int i = 0; i < MAX_USERS; i++) {
                    if (clients[i].sin_port > 0) {
                        strcpy(msg.users[msg.user_count++], client_name[i]);
                    }
                }
                sendto(sockfd, &msg, sizeof(Message), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
            }
        }
    } else {
        while (running) {}
    }
        
    kill(pid, SIGKILL);
    close(sockfd);
    return 0;
}