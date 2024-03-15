CC=gcc
cflags := -Wall

BUILD_DIR = build
TIME := /usr/bin/time
MEASUREMENTS := measurements.txt

.PHONY: all clean

all: reverse_byte reverse_block test_generator
	 

reverse_byte: reverse.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $< -D BYTE

reverse_block: reverse.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $< -D BLOCK

test_generator: test_generator.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(cflags) -o $(BUILD_DIR)/$@ $<

generate_tests: test_generator
	@ mkdir -p $(DIR)
	@ ./$(BUILD_DIR)/test_generator 512     $(DIR)/size1.txt
	@ ./$(BUILD_DIR)/test_generator 1024    $(DIR)/size2.txt
	@ ./$(BUILD_DIR)/test_generator 10000   $(DIR)/size3.txt
	@ ./$(BUILD_DIR)/test_generator 100000  $(DIR)/size4.txt
	@ ./$(BUILD_DIR)/test_generator 1000000 $(DIR)/size5.txt

validate: all generate_tests
	@ rev $(DIR)/size1.txt > $(DIR)/size1_rev.txt
	@ rev $(DIR)/size2.txt > $(DIR)/size2_rev.txt
	@ rev $(DIR)/size3.txt > $(DIR)/size3_rev.txt
	@ rev $(DIR)/size4.txt > $(DIR)/size4_rev.txt
	@ rev $(DIR)/size5.txt > $(DIR)/size5_rev.txt
	@ echo "Validating reverse_byte"
	@ ./$(BUILD_DIR)/reverse_byte $(DIR)/size1.txt $(DIR)/size1_byte.txt
	@ ./$(BUILD_DIR)/reverse_byte $(DIR)/size2.txt $(DIR)/size2_byte.txt
	@ ./$(BUILD_DIR)/reverse_byte $(DIR)/size3.txt $(DIR)/size3_byte.txt
	@ ./$(BUILD_DIR)/reverse_byte $(DIR)/size4.txt $(DIR)/size4_byte.txt
	@ ./$(BUILD_DIR)/reverse_byte $(DIR)/size5.txt $(DIR)/size5_byte.txt
	@ diff -s $(DIR)/size1_rev.txt $(DIR)/size1_byte.txt
	@ diff -s $(DIR)/size2_rev.txt $(DIR)/size2_byte.txt
	@ diff -s $(DIR)/size3_rev.txt $(DIR)/size3_byte.txt
	@ diff -s $(DIR)/size4_rev.txt $(DIR)/size4_byte.txt
	@ diff -s $(DIR)/size5_rev.txt $(DIR)/size5_byte.txt
	@ echo "Validating reverse_block"
	@ ./$(BUILD_DIR)/reverse_block $(DIR)/size1.txt $(DIR)/size1_block.txt
	@ ./$(BUILD_DIR)/reverse_block $(DIR)/size2.txt $(DIR)/size2_block.txt
	@ ./$(BUILD_DIR)/reverse_block $(DIR)/size3.txt $(DIR)/size3_block.txt
	@ ./$(BUILD_DIR)/reverse_block $(DIR)/size4.txt $(DIR)/size4_block.txt
	@ ./$(BUILD_DIR)/reverse_block $(DIR)/size5.txt $(DIR)/size5_block.txt
	@ diff -s $(DIR)/size1_rev.txt $(DIR)/size1_block.txt
	@ diff -s $(DIR)/size2_rev.txt $(DIR)/size2_block.txt
	@ diff -s $(DIR)/size3_rev.txt $(DIR)/size3_block.txt
	@ diff -s $(DIR)/size4_rev.txt $(DIR)/size4_block.txt
	@ diff -s $(DIR)/size5_rev.txt $(DIR)/size5_block.txt
	rm -rf $(DIR)

measure: all generate_tests
	@ echo "Measuring reverse_byte"
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size1.txt reverse_byte %e" ./$(BUILD_DIR)/reverse_byte $(DIR)/size1.txt $(DIR)/size1_byte.txt
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size2.txt reverse_byte %e" ./$(BUILD_DIR)/reverse_byte $(DIR)/size2.txt $(DIR)/size2_byte.txt
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size3.txt reverse_byte %e" ./$(BUILD_DIR)/reverse_byte $(DIR)/size3.txt $(DIR)/size3_byte.txt
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size4.txt reverse_byte %e" ./$(BUILD_DIR)/reverse_byte $(DIR)/size4.txt $(DIR)/size4_byte.txt
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size5.txt reverse_byte %e" ./$(BUILD_DIR)/reverse_byte $(DIR)/size5.txt $(DIR)/size5_byte.txt
	@ echo "Measuring reverse_block"
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size1.txt reverse_block %e" ./$(BUILD_DIR)/reverse_block $(DIR)/size1.txt $(DIR)/size1_block.txt
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size2.txt reverse_block %e" ./$(BUILD_DIR)/reverse_block $(DIR)/size2.txt $(DIR)/size2_block.txt
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size3.txt reverse_block %e" ./$(BUILD_DIR)/reverse_block $(DIR)/size3.txt $(DIR)/size3_block.txt
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size4.txt reverse_block %e" ./$(BUILD_DIR)/reverse_block $(DIR)/size4.txt $(DIR)/size4_block.txt
	@ $(TIME) -o $(MEASUREMENTS) -a -f "size5.txt reverse_block %e" ./$(BUILD_DIR)/reverse_block $(DIR)/size5.txt $(DIR)/size5_block.txt
	rm -rf $(DIR)

clean:
	rm -rf $(BUILD_DIR)
