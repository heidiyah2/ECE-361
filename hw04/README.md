# HW4: Tally ADT

`tally.h` / `tally.c` hold an opaque tally type that counts how often each name occurs.
The tallies live in a static pool of `TALLY_MAX` inside `tally.c`, and each tally keeps its
names sorted in lexicographical order. `events.c` reads names from standard input into one tally and
prints it. `tests/test_tally.c` holds the tests for the tally functions.

## Build

From the `hw04/` directory:

```sh
make
```

This builds `events` with `-std=c11 -Wall -Wextra -g -fsanitize=address,undefined`.

## Run

```sh
./events < data/door.txt
```

`events` reads one name per line, skips blank lines and lines that start with `#`, and adds
every other line to the tally. It prints one line per name as `name count` in alphabetical
order, then a last line `names: N, lines skipped: S`, where S counts the lines `tally_add` refused.

## Test

```sh
make test
```

This builds the tests if needed, then runs `./tests/test_tally`. It prints `PASS` or `FAIL`
for each check and then a summary. It exits with status 0 when every check passes and 1
when any check fails.

## Clean

```sh
make clean
```

This removes `events.o`, `tally.o`, `tests/test_tally.o`, `events`, and `tests/test_tally`.
