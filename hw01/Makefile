CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

.PHONY: all test clean

all: test_runner

test_runner: test.o bits.o
	$(CC) $(CFLAGS) -o $@ $^

test.o: test.c bits.h
	$(CC) $(CFLAGS) -c $< -o $@

bits.o: bits.c bits.h
	$(CC) $(CFLAGS) -c $< -o $@

test: test_runner
	./test_runner

clean:
	rm -f *.o test_runner
