#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

#include "pipeData.h"


double func(double x) {
    return 4 / (x * x + 1);
}


int main() {
    Config config;
    double result = 0;

    int input_pipe = open(INPUT_PIPE, O_RDONLY);
    int output_pipe = open(OUTPUT_PIPE, O_WRONLY);

    if (read(input_pipe, &config, sizeof(config)) < 0) {
        perror("Can't read from the input_pipe\n");
        return 1;
    }

    double width = (config.end - config.begin) / config.rect_num;
    for (int i=0; i<config.rect_num; i++) {
        result += func(config.begin + width * i);
    }
    result *= width;
    
    if (write(output_pipe, &result, sizeof(result)) < 0) {
        perror("Can't write to the output_pipe\n");
        return 1;
    }

    close(input_pipe);
    close(output_pipe);
    return 0;
}
