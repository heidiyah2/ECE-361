CC = gcc
# Handout: gcc -std=c11 -Wall -Wextra, no warnings, clean under the sanitizers.
CFLAGS = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined
LDLIBS = -lm

# Every module except main.c; the program and every test link these.
MODULES = aux.c fsm.c iom361_r4.c report.c sensor.c store.c
MODULE_OBJS = $(MODULES:.c=.o)

# One unit-test program per module, in tests/
TESTS = tests/test_sensor tests/test_fsm tests/test_store tests/test_aux tests/test_report

.PHONY: all test clean
# Keep tests/*.o (make deletes "intermediate" files by default), so an
# unchanged test is not recompiled on every make test.
.SECONDARY:

all: dishwasher

dishwasher: main.o $(MODULE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Each test program = its own .c file + all the modules
tests/test_%: tests/test_%.o $(MODULE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# -I. lets tests/*.c write #include "store.h" for headers in project/
%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

# Header dependencies: every header each .c file includes, directly or
# through another header. Update these whenever an #include changes.
main.o:   main.c sensor.h iom361_r4.h
aux.o:    aux.c aux.h
fsm.o:    fsm.c fsm.h
iom361_r4.o: iom361_r4.c iom361_r4.h
report.o: report.c report.h
sensor.o: sensor.c sensor.h iom361_r4.h
store.o:  store.c store.h sensor.h iom361_r4.h fsm.h

tests/test_sensor.o: tests/test_sensor.c sensor.h iom361_r4.h
tests/test_fsm.o:    tests/test_fsm.c fsm.h
tests/test_store.o:  tests/test_store.c store.h sensor.h iom361_r4.h fsm.h
tests/test_aux.o:    tests/test_aux.c aux.h
tests/test_report.o: tests/test_report.c report.h

# Builds dishwasher and every test, then runs each test.
# TODO: scenario tests, and the final passed/failed summary line.
test: dishwasher $(TESTS)
	@for t in $(TESTS); do ./$$t; done

clean:
	rm -f dishwasher main.o $(MODULE_OBJS) $(TESTS) tests/*.o
