#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// argc - number of arguments
// argv[0] - program name
// argv[1] - len of file
// argv[2] - file to write
int main(int argc, char* argv[]) {

    FILE* file = fopen(argv[2], "w");
    if(file == NULL){
        return -1;
    }
    //convert string argument into long
    long number_of_files = strtol(argv[1], NULL, 10);
    //generate wanted amount of bytes to the file
    for(int i = 0; i < number_of_files; i++) {
        fputc(rand()%('z' - 'a' + 1) + 'a', file);
    }
    //close output file
    fclose(file);
    return 0;
}