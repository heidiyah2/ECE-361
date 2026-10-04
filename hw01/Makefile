CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

.PHONY: all test clean

all: test_runner

test_runner: tests/test_bits.o bits.o status.o
	$(CC) $(CFLAGS) -o $@ $^

tests/test_bits.o: tests/test_bits.c bits.h status.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

bits.o: bits.c bits.h
	$(CC) $(CFLAGS) -c $< -o $@

status.o: status.c status.h bits.h
	$(CC) $(CFLAGS) -c $< -o $@

test: test_runner
	./test_runner

clean:
	rm -f *.o tests/*.o test_runner
