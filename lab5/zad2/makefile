CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean

all: sender catcher 
	 

sender: sender.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

catcher: catcher.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<


clean:
	rm -rf $(BUILD_DIR)
