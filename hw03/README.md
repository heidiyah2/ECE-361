# HW3: Pointer library

`ptr.h` / `ptr.c` hold four functions written with pointer arithmetic:
`swap_float`, `minmax`, `find_above`, and `reverse`. `tests/test_ptr.c` holds the tests for them.

## Build

From the `hw03/` directory:

```sh
make
```

This builds `ptr.o` and `tests/test_ptr` with `-std=c11 -Wall -Wextra -g -fsanitize=address,undefined`.

## Test

```sh
make test
```

This builds the program if needed, then runs `./tests/test_ptr`. It prints `PASS` or `FAIL`
for each test and then a summary. It exits with status 0 when every test passes and 1
when any test fails.

## Clean

```sh
make clean
```

This removes `ptr.o`, `tests/test_ptr.o`, and `tests/test_ptr`.
