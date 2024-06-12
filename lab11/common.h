#ifndef COMMON_H
#define COMMON_H

#define MAX_USERS 10
#define USERNAME_LEN 16
#define MAX_MESSAGE_LENGTH 128

typedef enum {
    LIST,
    TOALL,
    TOONE,
    STOP,
    ALIVE
} MessageType;

typedef struct {
    char to_user[USERNAME_LEN];
    char message[MAX_MESSAGE_LENGTH];
} toone_t;

typedef struct {
    char user_list[MAX_USERS][USERNAME_LEN];
    int user_count;
} users_list_t;

typedef struct {
    MessageType type;
    char from_user[USERNAME_LEN];
    union {
        toone_t toone;
        char toall[MAX_MESSAGE_LENGTH];
        users_list_t list;
    } data;
} Message;

typedef struct {
    int client_sockfd[MAX_USERS];
    char client_name[MAX_USERS][USERNAME_LEN];
} server_data_t;

#endif