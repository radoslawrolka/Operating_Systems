#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


double func(double x) {
    return 4 / (x * x + 1);
}

// argv[1] - ilość prostokątów
// argv[2] - ilość procesów
int main(int argc, char* argv[]) {
    long rect_num = atol(argv[1]);
    long proc_num = atol(argv[2]);
    double width = 1.0 / rect_num;

    int data[rect_num][2];
    long current = 0;
    for (int i=0; i<proc_num; i++) {
        long per_proc = rect_num / proc_num + (i < rect_num % proc_num);
        for (int j=0; j<per_proc; j++) {
            pipe(data[current+j]);
        }
        if (fork() == 0) {
            for (int j=0; j<per_proc; j++) {
                close(data[current+j][0]);
                double sum = func((current+j) * width);
                write(data[current+j][1], &sum, sizeof(sum));
                close(data[current+j][1]);
            }
            exit(0);
        }
        for (int j=0; j<per_proc; j++) {
            close(data[current++][1]);
        }
    }

    wait(NULL);
    double result = 0;
    for (int i=0; i<rect_num; i++) {
        double sum;
        read(data[i][0], &sum, sizeof(sum));
        result += sum;
        close(data[i][0]);
    }
    printf("%f\n", result * width);
    return 0;
}
