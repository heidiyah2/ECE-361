CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined

TEST_OBJECT = tests/test_ptr.o

.PHONY: all test clean

all: ptr.o tests/test_ptr

ptr.o: ptr.c ptr.h

tests/test_ptr: $(TEST_OBJECT) ptr.o
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJECT) ptr.o

tests/test_ptr.o: tests/test_ptr.c ptr.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: tests/test_ptr
	./tests/test_ptr

clean:
	rm -f ptr.o $(TEST_OBJECT) tests/test_ptr
