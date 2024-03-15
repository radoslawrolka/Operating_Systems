CC=gcc
cflags := -Wall

BUILD_DIR = build

.PHONY: all tests clean

all: file_sizes
	 

file_sizes: file_sizes.c
	@ mkdir -p $(BUILD_DIR)
	@ $(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

validate: all
	@ echo "Validating..."
	@ echo "Test file_sizes:"
	@ ./$(BUILD_DIR)/file_sizes
	@ echo "Test wc --bytes * :"
	@ wc --bytes *
	@ echo "Validation complete"

clean:
	@ rm -rf $(BUILD_DIR)
