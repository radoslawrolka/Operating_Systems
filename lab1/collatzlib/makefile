# C compiler command
CC=gcc
# defines path to put all binary files
BUILD_DIR=build

# C flags common in all build configurations
# -Wall - enable all warnings
# -std=c11 - set c standard to 11
cflags.common := -std=c11 -Wall
# build CFLAGS variable based on selected build configuration
CFLAGS := ${cflags.common}

.PHONY: all collatz_static collatz_shared clean

# build both static and shared library
all:
	make collatz_static
	make collatz_shared

# build only static library
# gcc options:
# 	-c - compile only, files will be only compiled, but not linked
#   $@ is the target name, so it becomes 'collatz_static'
# ar command packs compilation result (object file) into .a archive
# ar options:
#    r - insert with replacement, if archive exists and contains given file, file inside archive will be replaced
#	 c - create, creates archive if it does not exist
# 	 s - updates archive index						 
collatz_static: collatz.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<
	ar rcs $(BUILD_DIR)/lib$@.a $(BUILD_DIR)/$@

# build only shared library
# first we compile sources to object file with -fPIC option
# than we use GCC to crated shared object file (.so)
# gcc options:
# 	-c - compile only, files will be only compiled, but not linked
#   -fPIC - create Position Indexed Code, code accesses all constant adrress through special offset table (Global Offset Table) so it can be loaded dynamically on the start of the program
#   -shared - create shared object file
collatz_shared: collatz.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -fPIC -o $(BUILD_DIR)/$@.o -c $<
	$(CC) -shared -o $(BUILD_DIR)/lib$@.so $(BUILD_DIR)/$@.o

clean:
	rm -rf $(BUILD_DIR)