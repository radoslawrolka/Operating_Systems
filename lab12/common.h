#ifndef COMMON_H
#define COMMON_H

#define MAX_USERS 10
#define USERNAME_LEN 16
#define MAX_MESSAGE_LENGTH 128

typedef enum {
    TOALL,
    TOONE,
    LIST,
    STOP,
    ALIVE
} MessageType;

typedef struct {
    MessageType type;
    char from_user[USERNAME_LEN];
    char to_user[USERNAME_LEN];
    char message[MAX_MESSAGE_LENGTH];
    char users[MAX_USERS][USERNAME_LEN];
    int user_count;
} Message;

#endif