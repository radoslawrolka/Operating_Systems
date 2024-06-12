CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean clear

all: mutex

server: server.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<
	
client: client.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

run_client1: client
	./$(BUILD_DIR)/client 127.0.0.1 5000 first

run_client2: client
	./$(BUILD_DIR)/client 127.0.0.1 5000 second

run_client3: client
	./$(BUILD_DIR)/client 127.0.0.1 5000 third

run_server: server
	./$(BUILD_DIR)/server 127.0.0.1 5000
	
clean:
	rm -rf $(BUILD_DIR)

clear:
	rm -rf $(BUILD_DIR)