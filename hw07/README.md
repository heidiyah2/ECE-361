# HW7: A Growing Stack and a Bracket Checker

`balance` reads standard input one character at a time and checks that every `(`, `[`, and `{`
is closed by the matching `)`, `]`, or `}`, in the right order. It keeps the open brackets on
`istack`, a stack of `int` on the heap that starts with room for 8 values and doubles when full.

| File | What it does |
|---|---|
| `istack.h` / `istack.c` | the `istack` ADT: create, destroy, push, pop, peek, count, is_empty, capacity, to_array |
| `balance.c` | the bracket checker |
| `tests/test_istack.c` | unit tests for `istack` |
| `inputs/`, `expected/`, `regress.sh` | the regression test for `balance` |

## Build

From the `hw07/` directory:

```sh
make
```

This builds `balance` with `-std=c11 -Wall -Wextra -g`, without sanitizers.

## Run

```sh
./balance < inputs/balanced.txt
```

Every bracket counts, including those in comments and strings. `balance` stops at the first error
and prints exactly one line to standard output. Lines and columns count from 1.

| Result | Output | Exit status |
|---|---|---|
| balanced | `balanced: 6 pairs, deepest 2` | 0 |
| wrong closer | `line 1, column 8: found ')', expected ']'` | 1 |
| closer with nothing open | `line 1, column 12: found ')', nothing open` | 1 |
| input ends with brackets open | `end of input: 2 still open, innermost '('` | 1 |

`deepest` is the most brackets open at once. An empty input is balanced with 0 pairs, deepest 0.

## Test

```sh
make test
```

This builds `balance` and the unit tests if needed, then:

1. runs `./tests/test_istack`, the unit tests for `istack`. It prints `PASS` or `FAIL` for each
   check and a summary.
2. runs `sh regress.sh`, the regression test. For each file in `inputs/` it runs
   `./balance < inputs/name.txt` and compares the output and exit status with
   `expected/name.txt`, printing `PASS` or `FAIL` per file, with the difference on a failure.

`make test` exits with a nonzero status if any unit test or regression test fails. Run it from
`hw07/`, because `regress.sh` uses paths relative to it.

To add a regression test, put the input in `inputs/name.txt` and, in `expected/name.txt`, the one
line `balance` should print followed by `exit status N`.

## Memory check

```sh
make memcheck
```

This removes all build files, rebuilds everything with `-fsanitize=address,undefined`, and runs
`make test`. AddressSanitizer reports invalid memory use, and on Linux LeakSanitizer reports
leaks at exit. Either one makes the run fail. Afterwards it removes the sanitized build files again,
so a later plain `make` doesn't mix sanitized and plain object files.

On macOS, LeakSanitizer is not available. Find leaks with a plain build instead:

```sh
make clean && make tests/test_istack
leaks --atExit -- ./tests/test_istack
```

## Clean

```sh
make clean
```

This removes the `.o` files, `balance`, and `tests/test_istack`.

## Part 4: Breaking it on purpose

1. I added an extra `istack_destroy(s)` at the end of `balance.c`, then ran `make memcheck`. The first line of the error report was:

```sh
==95816==ERROR: AddressSanitizer: heap-use-after-free on address 0x503000000040 at pc 0x5617493eabaa bp 0x7ffdf3377b50 sp 0x7ffdf3377b48
```

Free was already used once, which told the allocator that the memory at `s` could be used again, so attempting to use it in any way is dangerous and produces an error with the sanitizer since the 2nd call of `free` attempts to access `s` after the memory at that address has been marked as free.

2. I added `istack_push(s, 1);` to `balance.c` after freeing `s`:

```sh
==103515==ERROR: AddressSanitizer: heap-use-after-free on address 0x503000000048 at pc 0x55d049f43c63 bp 0x7ffd22ca51e0 sp 0x7ffd22ca51d8
```

The same error is given because again, the memory `s` points to was marked as free, and trying to modify it in any way afterwards is a `use-after-free` due to the dangling pointer.

3. I took out the `istack_destroy(s)` at the end of `balance.c`:

```sh
==104905==ERROR: LeakSanitizer: detected memory leaks
```

This time `s` was not freed, so `s` disappears with the stack frame and the memory location `s` pointed to is no longer accessible, which is a memory leak.
