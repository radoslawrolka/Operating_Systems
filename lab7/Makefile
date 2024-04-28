CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean

all: server client

server: server.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

client: client.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<


clean:
	rm -rf $(BUILD_DIR)
