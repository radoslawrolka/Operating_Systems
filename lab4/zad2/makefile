CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean

all: exec_child
	 

exec_child: exec_child.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

clean:
	rm -rf $(BUILD_DIR)
