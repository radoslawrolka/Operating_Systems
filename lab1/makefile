CXX = gcc
CFLAGS = -Wall

.PHONY: all countdown clean

all: countdown

countdown: countdown.c
	$(CXX) $(CFLAGS) -o countdown.o countdown.c

clean:
	rm -f *.o