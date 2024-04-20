
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int fd[2];
int val = 0;
/*program tworzy dwa procesy potomne.
 * Proces macierzysty co sekund� wysy�a SIGUSR1 do procesu potomnego 1.
 *
 * Proces potomny 1 po otrzymaniu sygna�u przesy�a kolejn� liczb� przez potok nienazwany do procesu potomnego 2,
 *
 * kt�ry wyswietla te liczbe */

//
//
void handler(int sig) {
    int val += 1; //jaką liczbe mam przesyłać ? kolejną? ale kolejną od czego?
    write(fd[1], &val, sizeof (val));
}


int main(int lpar, char *tab[]) {
    pid_t pid1, pid2;
    int d, i;
    //
    pid1 = fork();

    if (pid1 < 0) {
        perror("fork");
        return 0;
    } else if (pid1 == 0) {//proces 1
        close(fd[0]);
        signal(handler, SIGUSR1);
        while (1);
        return 0;
    } else {
        //
        pid2 = fork();
        if (pid2 < 0) {
            perror("fork");
            return 0;
        } else if (pid2 == 0) {//proces 2
            close(fd[1]);
            while (1) {
                d = read(fd[0], &i, sizeof (i));
                printf("przyjeto %d bajtow, wartosc:%d\n", d, i);
            }
            return 0;
        }
    }
    close(fd[0]);
    close(fd[1]);
    while (1) {
        //
        kill(pid1, SIGUSR1);
        printf("wyslano SIGUSR1\n");
        sleep(1);
    }
}
