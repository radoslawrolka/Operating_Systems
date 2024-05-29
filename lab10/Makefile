CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean

all: mutex

mutex: mutex.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $< -lpthread
	
clean:
	rm -rf $(BUILD_DIR)

clear:
	rm -rf $(BUILD_DIR)