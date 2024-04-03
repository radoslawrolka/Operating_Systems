#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>


void sigusr1_handler(int signum) {
    printf("Signal confirmed");
}

// argv[1] - catcher PID
// argv[2] - mode
int main(int argc, char* argv[]) {
    signal(SIGUSR1, sigusr1_handler);
    
    pid_t catcher_pid = atoi(argv[1]);
    int mode = atoi(argv[2]);

    sigqueue(catcher_pid, SIGUSR1, (union sigval) {mode});
    return 0;
}