CC = cc
CFLAGS = -std=c11 -Wall -Wextra

MODULE_OBJECTS = main.o reading.o stats.o histogram.o
TEST_OBJECT = tests/test_stats.o

.PHONY: all test clean

all: readings

readings: $(MODULE_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(MODULE_OBJECTS)

main.o: main.c reading.h stats.h histogram.h
reading.o: reading.c reading.h
stats.o: stats.c stats.h
histogram.o: histogram.c histogram.h

test: readings tests/test_stats
	./tests/test_stats; unit_status=$$?; \
	sh regress.sh; regression_status=$$?; \
	test $$unit_status -eq 0 && test $$regression_status -eq 0

tests/test_stats: $(TEST_OBJECT) stats.o
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJECT) stats.o

tests/test_stats.o: tests/test_stats.c stats.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(MODULE_OBJECTS) $(TEST_OBJECT) readings tests/test_stats
