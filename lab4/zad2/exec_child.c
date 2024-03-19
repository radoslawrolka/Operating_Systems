#include <stdio.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int global = 0;

// argv[1] - dir path
int main(int argc, char* argv[]) {
    printf("Program name: %s\n", argv[0]);
    int local = 0;

    pid_t child_pid=fork();
    if (child_pid == 0) {
        printf("child process\n");
        global++;
        local++;
        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n", local, global);
        exit(execl("/bin/ls", "ls", argv[1], NULL));
    } else {
        int status, childreturned;
        wait(&status);
        childreturned = WEXITSTATUS(status);
        printf("parent process\n");
        printf("parent pid = %d, child pid = %d\n", getpid(), child_pid);
        printf("Child exit code: %d\n", childreturned);
        printf("parent's local = %d, parent's global = %d\n", local, global);
        return childreturned;
    }
}
