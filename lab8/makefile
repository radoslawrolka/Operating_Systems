CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean

all:integral

users: users.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $< -lrt -lpthread

printers: printers.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $< -lrt -lpthread
	
clean:
	rm -rf $(BUILD_DIR)

clear:
	rm -rf $(BUILD_DIR)