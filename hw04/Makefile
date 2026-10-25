CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined

OBJECTS = events.o tally.o
TEST_OBJECT = tests/test_tally.o

.PHONY: all test clean

all: events

events: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

events.o: events.c tally.h
tally.o: tally.c tally.h

tests/test_tally: $(TEST_OBJECT) tally.o
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJECT) tally.o

tests/test_tally.o: tests/test_tally.c tally.h
	$(CC) $(CFLAGS) -c -o $@ $<

test: tests/test_tally
	./tests/test_tally

clean:
	rm -f $(OBJECTS) $(TEST_OBJECT) events tests/test_tally
