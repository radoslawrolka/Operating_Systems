CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean

all: first second

first: first.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

second: second.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

# use -j flag to parallelize the execution of the two programs
run: all
	./$(BUILD_DIR)/first 0 1 100 & ./$(BUILD_DIR)/second

clean:
	rm -rf $(BUILD_DIR)
	rm input_pipe output_pipe
