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

#include "common.h"

volatile bool running = true;
void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("Received SIGINT\n");
        running = false;
    }
}


// argv[1] = server_ip
// argv[2] = server_port
// argv[3] = username
int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Usage: %s <server_ip> <server_port> <username>\n", argv[0]);
        return 1;
    }
    signal(SIGINT, sig_handler);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return 1;
    }
    Message msg;
    msg.type = ALIVE;
    strcpy(msg.from_user, argv[3]);
    if (send(sockfd, &msg, sizeof(Message), 0) < 0) {
        perror("send");
        return 1;
    }
    if (recv(sockfd, &msg, sizeof(Message), 0) < 0) {
        perror("recv");
        return 1;
    }
    if (msg.type == ALIVE) {
        printf("Connected to server\n");
    } else {
        printf("Failed to connect to server\n");
        return 1;
    }
    
    if (fork() == 0) { // all
        if (fork() != 0) { // sending messages
            while (running) {
                char buffer[MAX_MESSAGE_LENGTH];
                printf("ENTER TYPE: ");
                fgets(buffer, MAX_MESSAGE_LENGTH, stdin);
                buffer[strlen(buffer)-1] = '\0';
                if (strcmp(buffer, "all") == 0) {
                    Message msg;
                    msg.type = TOALL;
                    strcpy(msg.from_user, argv[3]);
                    printf("Enter message: ");
                    fgets(msg.message, MAX_MESSAGE_LENGTH, stdin);
                    msg.message[strlen(msg.message) - 1] = '\0';
                    send(sockfd, &msg, sizeof(Message), 0);
                } else if (strncmp(buffer, "to ", 3) == 0) {
                    Message msg;
                    msg.type = TOONE;
                    strcpy(msg.from_user, argv[3]);
                    char* to_user = strtok(buffer + 3, " ");
                    strcpy(msg.to_user, to_user);
                    printf("Enter message: ");
                    fgets(msg.message, MAX_MESSAGE_LENGTH, stdin);
                    msg.message[strlen(msg.message) - 1] = '\0';
                    send(sockfd, &msg, sizeof(Message), 0);
                } else if (strcmp(buffer, "list") == 0) {
                    Message msg;
                    msg.type = LIST;
                    send(sockfd, &msg, sizeof(Message), 0);
                }
            }
        } else { // receiving messages
            while (running) {
                if (recv(sockfd, &msg, sizeof(Message), 0) > 0) {
                    switch (msg.type) {
                        case TOALL:
                            printf("TOALL %s: \n%s\n", msg.from_user, msg.message);
                            break;
                        case TOONE:
                            printf("TOONE %s->me:\n %s\n", msg.from_user, msg.message);
                            break;
                        case ALIVE:
                            send(sockfd, &msg, sizeof(Message), 0);
                            break;
                        case LIST:
                            printf("LIST:\n");
                            for (int i = 0; i < msg.user_count; i++) {
                                printf("%s\n", msg.users[i]);
                            }
                            break;
                        case STOP:
                            running = false;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    } else { // closing client
        while (running) {}
    }

    close(sockfd);
    return 0;
}
