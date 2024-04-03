#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

// SIGUSR1
void signal_handler(int signum) {
    printf("Signal SIGUSR1 %d received\n", signum);
}

// argc - argument count
// argv[0] - program name
// argv[1] - none / ignore / handler / mask
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments\n");
        return 1;
    }

    if (strcmp(argv[1], "none") == 0) {
        signal(SIGUSR1, SIG_DFL);
        raise(SIGUSR1);
    } else if (strcmp(argv[1], "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
        raise(SIGUSR1);
    } else if (strcmp(argv[1], "handler") == 0) {
        signal(SIGUSR1, signal_handler);
        raise(SIGUSR1);
    } else if (strcmp(argv[1], "mask") == 0) {
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_SETMASK, &mask, NULL);
        raise(SIGUSR1);
        
        sigset_t pending;
        sigpending(&pending);
        printf("Is signal SIGUSR1 pending: %i\n", sigismember(&pending, SIGUSR1));
    } else {
        printf("Invalid argument\n");
        return 1;
    }
    
    return 0;
}