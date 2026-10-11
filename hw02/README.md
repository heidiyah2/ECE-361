# HW02

## Building

From this directory, build the program with:

```sh
make
```

This creates the `readings` executable.

To remove compiled object files and executables:

```sh
make clean
```

## Testing

Run the unit tests and regression tests with:

```sh
make test
```

The unit tests run first, followed by `regress.sh` for the regression tests
against the files in `data/` and `expected/`. `make test` returns a nonzero status if either suite
fails.

The program reads input from standard input. For example:

```sh
./readings < data/normal.txt
./readings 25 < data/edge.txt
```

## Module diagram

An arrow means that the module on the left calls a function provided by the
module on the right.

```text
+------------------+       +------------------+
|     main.c       |------>|    reading.c     |
|  program entry   |       |  read_lines()    |
+------------------+       +------------------+
        |
        +----------------->+------------------+
        |                   |     stats.c      |
        |                   |  stats functions |
        |                   +------------------+
        |
        +----------------->+------------------+
                            |   histogram.c    |
                            |   histogram()    |
                            +------------------+

+------------------+       +------------------+
| tests/test_stats |------>|     stats.c      |
|    unit tests    |       |  stats functions |
+------------------+       +------------------+
```

The header files declare the interfaces used between modules:

- `reading.h` declares `read_lines()`.
- `stats.h` declares the statistics functions.
- `histogram.h` declares `histogram()`.
