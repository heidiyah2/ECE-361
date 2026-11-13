CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g
SANITIZE = -fsanitize=address,undefined

.PHONY: all test memcheck clean

all: balance

balance: balance.o istack.o
	$(CC) $(CFLAGS) -o $@ balance.o istack.o

balance.o: balance.c istack.h
istack.o: istack.c istack.h

tests/test_istack: tests/test_istack.o istack.o
	$(CC) $(CFLAGS) -o $@ tests/test_istack.o istack.o

tests/test_istack.o: tests/test_istack.c istack.h
	$(CC) $(CFLAGS) -I. -c -o $@ $<

test: balance tests/test_istack
	./tests/test_istack
	sh regress.sh

# rebuild everything with the sanitizers, then run the tests; clean again
# afterwards (keeping the test's exit status) so no sanitized object is left
# behind to mix with a later plain build
memcheck:
	$(MAKE) clean
	$(MAKE) test CFLAGS="$(CFLAGS) $(SANITIZE)"; status=$$?; $(MAKE) clean; exit $$status

clean:
	rm -f balance balance.o istack.o tests/test_istack tests/test_istack.o
