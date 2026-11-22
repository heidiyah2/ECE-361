CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
SANITIZE = -fsanitize=address,undefined

.PHONY: all test memcheck clean

all: treestats

treestats: treestats.o bst.o
	$(CC) $(CFLAGS) -o $@ treestats.o bst.o

treestats.o: treestats.c bst.h
bst.o: bst.c bst.h

tests/test_bst: tests/test_bst.o bst.o
	$(CC) $(CFLAGS) -o $@ tests/test_bst.o bst.o

tests/test_bst.o: tests/test_bst.c bst.h
	$(CC) $(CFLAGS) -I. -c -o $@ $<

test: treestats tests/test_bst
	./tests/test_bst

# rebuild everything with the sanitizers, then run the tests; clean again
# afterwards (keeping the test's exit status) so no sanitized object is left
# behind to mix with a later plain build
memcheck:
	$(MAKE) clean
	$(MAKE) test CFLAGS="$(CFLAGS) $(SANITIZE)"; status=$$?; $(MAKE) clean; exit $$status

clean:
	rm -f treestats treestats.o bst.o tests/test_bst tests/test_bst.o
