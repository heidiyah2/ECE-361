CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined
LDLIBS = -lm

OBJECTS = main.o sensor.o panel.o scenario.o iom361_r4.o
TEST_OBJECTS = tests/test_hw06.o sensor.o panel.o scenario.o iom361_r4.o

.PHONY: all test clean

all: replay

replay: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

main.o: main.c iom361_r4.h sensor.h panel.h scenario.h
sensor.o: sensor.c sensor.h iom361_r4.h
panel.o: panel.c panel.h iom361_r4.h
scenario.o: scenario.c scenario.h
iom361_r4.o: iom361_r4.c iom361_r4.h

test: replay tests/test_hw06
	./tests/test_hw06
	sh regress.sh

tests/test_hw06: $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJECTS) $(LDLIBS)

tests/test_hw06.o: tests/test_hw06.c iom361_r4.h sensor.h panel.h scenario.h
	$(CC) $(CFLAGS) -I. -c -o $@ $<

clean:
	rm -f $(OBJECTS) tests/test_hw06.o replay tests/test_hw06
