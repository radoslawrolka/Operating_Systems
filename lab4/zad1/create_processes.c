#include <stdio.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// argv[1] - number of processes to create
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return -1;
    }

    int n = atoi(argv[1]);
    for (int i = 0; i < n; i++) {
        if (fork() == 0) {
            printf("PID: %d, PPID: %d\n", getpid(), getppid());
            return 0;
        }
    }

    while (wait(NULL) > 0) {
        printf("Wait\n");
    }
    printf("All children have terminated\n");
    printf("Created %s processes\n", argv[1]);

    return 0;
}