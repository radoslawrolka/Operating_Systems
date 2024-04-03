CC=gcc
cflags := -Wall
BUILD_DIR = build

.PHONY: all clean

all: signal_reaction test
	 

signal_reaction: signal_reaction.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

test: signal_reaction
	./$(BUILD_DIR)/signal_reaction none
	./$(BUILD_DIR)/signal_reaction ignore
	./$(BUILD_DIR)/signal_reaction handler
	./$(BUILD_DIR)/signal_reaction mask

clean:
	rm -rf $(BUILD_DIR)
