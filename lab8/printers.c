#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

#include "common_data.h"

// argv[1] = number of printers
int main(int argc, char* argv[]) {
    int shared_memory_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    truncate(shared_memory_fd, sizeof(shared_memory_t));
    shared_memory_t* shared_memory = mmap(NULL, sizeof(shared_memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);

    int printers_number = atoi(argv[1]);
    shared_memory->printer_count = printers_number;

    for (int i = 0; i < printers_number; i++) {
        sem_init(&shared_memory->printers[i].semaphore, 1, 1);
        if (fork() == 0) {
            while (true) {
                if (shared_memory->printers[i].state == BUSY) {
                    for (int j = 0; j < shared_memory->printers[i].length; j++) {
                        printf("Printer %d is printing: %c  |  (%d / %d)\n", i, shared_memory->printers[i].buffer[j], j + 1, shared_memory->printers[i].length);
                        sleep(1);
                    }
                    shared_memory->printers[i].state = EMPTY;
                    sem_post(&shared_memory->printers[i].semaphore);
                }
            }
            exit(0);
        }
    }

    for (int i = 0; i < printers_number; i++) {
        wait(NULL);
        sem_destroy(&shared_memory->printers[i].semaphore);
    }
    munmap(shared_memory, sizeof(shared_memory_t));
    shm_unlink(SHARED_MEMORY_NAME);
    return 0;
}