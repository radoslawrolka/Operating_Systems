CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean

all: create_processes
	 

create_processes: create_processes.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

clean:
	rm -rf $(BUILD_DIR)
