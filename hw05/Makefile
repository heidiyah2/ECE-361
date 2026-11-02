CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined

OBJECTS = main.o lock.o
TEST_OBJECT = tests/test_lock.o
BONUS_OBJECTS = check.o tests/test_check.o

.PHONY: all test bonus clean

all: lock

lock: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o: main.c lock.h
lock.o: lock.c lock.h
check.o: check.c check.h lock.h

tests/test_lock: $(TEST_OBJECT) lock.o
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJECT) lock.o

tests/test_lock.o: tests/test_lock.c lock.h
	$(CC) $(CFLAGS) -c -o $@ $<

tests/test_check: $(BONUS_OBJECTS) lock.o
	$(CC) $(CFLAGS) -o $@ $(BONUS_OBJECTS) lock.o

tests/test_check.o: tests/test_check.c check.h lock.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: tests/test_lock
	./tests/test_lock

bonus: tests/test_check
	./tests/test_check

clean:
	rm -f $(OBJECTS) $(TEST_OBJECT) $(BONUS_OBJECTS) lock tests/test_lock tests/test_check
