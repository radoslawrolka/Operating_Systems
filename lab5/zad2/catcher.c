#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

volatile int signals_counter = 0;
volatile int status = 0;

void sigusr1_handler(int signum, siginfo_t* info, void *context) {
    signals_counter++;
    status = info->si_int;
    printf("Signal %d sent by PID: %d\n", status, info->si_pid);
    kill(info->si_pid, SIGUSR1);
}

int main() {

    struct sigaction act{};
    act.sa_sigaction = sigusr1_handler;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);

    printf("Catcher PID: %d\n", getpid());

    while (1) {
        switch (status) {
            case 1:
                status = 0;
                for (int i=0; i<100; i++) {
                    printf("%i ", i);
                }
                printf("\n");
                break;
            case 2:
                status = 0;
                printf("Received %d signals\n", signals_counter);;
                break;
            case 3:
                exit(0);
                break;
            default:
                break;
        }
    }

    return 0;
}