CC=gcc
cflags := -Wall
BUILD_DIR = build
TIME = /usr/bin/time

.PHONY: all clean

all:integral count

integral: integral.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

test: integral
	./$(BUILD_DIR)/main 100 100
	./$(BUILD_DIR)/main 1000 100
	./$(BUILD_DIR)/main 10000 100

count: integral
	$(TIME) ./$(BUILD_DIR)/integral 1000000 1
	$(TIME) ./$(BUILD_DIR)/integral 1000000 10
	$(TIME) ./$(BUILD_DIR)/integral 1000000 100
	$(TIME) ./$(BUILD_DIR)/integral 1000000 1000

clean:
	rm -rf $(BUILD_DIR)
