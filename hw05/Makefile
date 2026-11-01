CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined

OBJECTS = main.o lock.o
TEST_OBJECT = tests/test_lock.o

.PHONY: all test clean

all: lock

lock: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o: main.c lock.h
lock.o: lock.c lock.h

tests/test_lock: $(TEST_OBJECT) lock.o
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJECT) lock.o

tests/test_lock.o: tests/test_lock.c lock.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: tests/test_lock
	./tests/test_lock

clean:
	rm -f $(OBJECTS) $(TEST_OBJECT) lock tests/test_lock
