#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>


int main() {
    DIR* current_dir = opendir(".");
    if (!current_dir) {
        return -1;
    }

    struct dirent* entry;
    struct stat file_stat;
    long long total_size = 0;

    while ((entry = readdir(current_dir))) {

        if (stat(entry->d_name, &file_stat)==0) {
            if (!S_ISDIR(file_stat.st_mode)) {
                printf("%7ld %s\n", file_stat.st_size, entry->d_name);
                total_size += file_stat.st_size;
            }
        }       
    }
    printf("%7lld total\n", total_size);
    closedir(current_dir);
    return 0;
}
