#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pipeData.h"


void unlink_pipes() {
    unlink(INPUT_PIPE);
    unlink(OUTPUT_PIPE);
}

// argv[1] - start of interval
// argv[2] - end of interval
// argv[3] - number of rectangles
int main(int argc, char* argv[]) {
    double begin = atof(argv[1]);
    double end = atof(argv[2]);
    long rect_num = atol(argv[3]);
    Config config = {begin, end, rect_num};

    if (mkfifo(INPUT_PIPE, S_IRWXU) != 0) {
        printf("Can't initialize input_pipe\n");
        return 1;
    }
    if (mkfifo(OUTPUT_PIPE, S_IRWXU) != 0) {
        printf("Can't initialize output_pipe\n");
        unlink_pipes();        
        return 1;
    }

    int input_pipe = open(INPUT_PIPE, O_WRONLY);
    if (write(input_pipe, &config, sizeof(config)) < 0) {
        perror("Can't write to the input_pipe\n");
        unlink_pipes();
        return 1;
    }
    double result = 0;
    int output_pipe = open(OUTPUT_PIPE, O_RDONLY);
    if (read(output_pipe, &result, sizeof(result)) < 0) {
        perror("Can't read from the output_pipe\n");
        unlink_pipes();
        return 1;
    }

    close(input_pipe);
    close(output_pipe);
    unlink_pipes();

    printf("%f\n", result);
    return 0;
}