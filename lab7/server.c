#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#include "config.h"


int main() {
    struct mq_attr attributes = {
        .mq_flags = 0,
        .mq_msgsize = sizeof(message_t),
        .mq_maxmsg = 10
    };
    
    mqd_t server = mq_open(SERVER_PATH, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR, &attributes);
    if(server < 0)
        perror("mq_open server");
    
    mqd_t clients[5];
    for (int i = 0; i < 5; i++)
        clients[i] = -1;

    message_t message;
    while (true) {
        mq_receive(server, (char*)&message, sizeof(message), NULL);
        if (message.type == INIT) {
            for (int i = 0; i < 5; i++) {
                if (clients[i] == -1) {
                    clients[i] = mq_open(message.text, O_RDWR, S_IRUSR | S_IWUSR, NULL);
                    message.id = i;
                    mq_send(clients[i], (char*)&message, sizeof(message), 0);
                    printf("Client %d connected\n", i);
                    break;
                }
            }
        }
        else if (message.type == MESG) {
            for (int i = 0; i < 5; i++) {
                if (clients[i] != -1 && i != message.id) {
                    mq_send(clients[i], (char*)&message, sizeof(message), 0);
                }
            }
        }
        else if (message.type == END) {
            for (int i = 0; i < 5; i++) {
                if (clients[i] != -1) {
                    mq_send(clients[i], (char*)&message, sizeof(message), 0);
                    mq_close(clients[i]);
                    clients[i] = -1;
                }
            }
            break;
        }
    }
    printf("Server closed\n");
    mq_close(server);
    mq_unlink(SERVER_PATH);
    return 0;
}