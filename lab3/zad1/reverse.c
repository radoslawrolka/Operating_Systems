#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#if defined(BYTE)

// reverse file using byte by byte
void reverse(FILE* input, FILE* output) {
    // move to the end of the file
    fseek(input, -1, SEEK_END);
    // get the size of the file
    long size = ftell(input);
    // allocate buffer for one byte
    char buffer[1];
    // read bytes from the end of the file
    while (size >= 0) {
        // read one byte from the current position
        fread(buffer, 1, 1, input);
        // write bytes to the output file
        fwrite(buffer, 1, 1, output);
        // move to the previous byte
        fseek(input, -2, SEEK_CUR);
        size--;
    }
}

#elif defined(BLOCK)

#define BLOCK_SIZE 1024
// reverse file using block by block
void reverse(FILE* input, FILE* output) {
    // move to the end of the file
    fseek(input, 0, SEEK_END);
    // get the size of the file
    long size = ftell(input);
    // allocate buffer for BLOCK_SIZE bytes
    char buffer[BLOCK_SIZE];
    // read bytes from the end of the
    while (size > 0) {
        // read from current position to the beginning of the file
        long read_size = size > BLOCK_SIZE ? BLOCK_SIZE : size;
        fseek(input, -read_size, SEEK_CUR);
        fread(buffer, 1, read_size, input);
        // write reversed bytes to the output file
        for (int i = read_size-1; i >= 0; i--) {
            fwrite(buffer+i, 1, 1, output);
        }
        fseek(input, -read_size, SEEK_CUR);
        size -= read_size;
    }
}

#endif
// argc - number of arguments
// argv - array of arguments
// argv[0] - program name
// argv[1] - file to read
// argv[2] - file to write
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input> <output>\n", argv[0]);
        return -1;
    }

    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("input fopen");
        return -2;
    }
    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        perror("output fopen");
        return -3;
    }

    reverse(input, output);
    fclose(input);
    fclose(output);        
    return 0;
}
