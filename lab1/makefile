# C compiler command
CC=gcc
# defines path to put all binary files
BUILD_DIR=build

# C flags common in all build configurations
# -Wall - enable all warnings
# -std=c11 - set c standard to 11
cflags.common := -std=c11 -Wall 
# build CFLAGS variable 
CFLAGS := ${cflags.common}

.PHONY: all client_static client_shared_so client_shared_dlopen clean

# create all client executables
all:
	make client_static
	make client_shared_so
	make client_shared_dlopen

# create client executable which uses staticly linked library
# first compile static library if it doesn't exist
# to link object file with static library we just need to pass path to static library (.a file) at the end of gcc command
client_static: client.c
	mkdir -p $(BUILD_DIR)
	make -C collatzlib collatz_static
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< collatzlib/build/libcollatz_static.a

# create client dynamically linked at compilation (using so file)
# first compile shared library if it doesn't exist
# then compile executable linked to .so file
# gcc options:
#     -L <path> -specify path where linked library can be found 
# 	  -lcollatz_shared -specify library to be linked -lcollatz_shared means that linker will look for file named libcollatz_shared.so in <path> specified in -L option
#     -Wl  - option passing arguments to linker
# 	  -Wl,-rpath=<path>  - pass -rpath=<path> argument to linker, rpath itself is path for runtime library search, so the program knows where to look for .so file
client_shared_so: client.c
	mkdir -p $(BUILD_DIR)
	make collatz_shared -C collatzlib
	$(CC) $(CFLAGS) -L ./collatzlib/build $< -lcollatz_shared -Wl,-rpath=./collatzlib/build -o $(BUILD_DIR)/$@ 

# create client that uses dlopen for use of shared library
# just conventional gcc build with -ldl flag to link libdl used by dlopen
# -D flag passses define macro for the preprocessor
client_shared_dlopen: client.c
	mkdir -p $(BUILD_DIR)
	make collatz_shared -C collatzlib
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -ldl -D DYNAMIC_DLOPEN 

clean:
	rm -rf $(BUILD_DIR)
	make -C collatzlib clean