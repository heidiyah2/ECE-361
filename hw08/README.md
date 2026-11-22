# HW8: Porting a Binary Search Tree

`bst` is a binary search tree of `int` keys, ported from TutorialsPoint's `BinarySearchTree.c` into
an ADT with no globals. Smaller keys go left and all others go right, so an equal key is kept, to
the right. `treestats` reads integers, inserts them in order, and reports the tree's size and height.

| File | What it does |
|---|---|
| `bst.h` / `bst.c` | the `bst` ADT: create, destroy, insert, contains, size, height |
| `treestats.c` | reads integers from standard input and prints the tree's size and height |
| `tests/test_bst.c` | unit tests for `bst` |
| `BinarySearchTree.c` | the original code the port came from (not built) |

## Build

From the `hw08/` directory:

```sh
make
```

This builds `treestats` with `-std=c11 -Wall -Wextra -g`, without sanitizers.

## Run

```sh
./treestats < file
```

`treestats` reads integers separated by whitespace until the end of the input, inserts them into a
tree in that order, and prints exactly one line to standard output.

| Result | Output | Exit status |
|---|---|---|
| one or more integers | `n 7, height 3` | 0 |
| no input | `empty tree` | 0 |
| something that is not an integer | `not a number after 4 values` | 1 |

The height is the number of nodes on the longest path from the root down. For an error, the
number is how many integers were read before the bad input.

## Test

```sh
make test
```

This builds `treestats` and the unit tests if needed, then runs `./tests/test_bst`. It prints
`PASS` or `FAIL` for each test and exits with a nonzero status if any test fails. The tests
cover:

- searching an empty tree (the input that crashed the faithful port)
- the original's seven keys `27 14 35 10 19 31 42`: size 7, height 3, 31 found in 3 visits,
  15 not found in 3
- a duplicate key is kept: size 2
- keys inserted in increasing order: height equals the number of keys
- destroying a full tree

## Memory check

```sh
make memcheck
```

This removes all build files, rebuilds everything with `-fsanitize=address,undefined`, and runs
`make test`. AddressSanitizer reports invalid memory use, UndefinedBehaviorSanitizer reports
undefined behavior such as following a NULL pointer, and on Linux LeakSanitizer reports leaks at
exit. Any of these makes the run fail. Afterwards it removes the sanitized build files again, so a
later plain `make` doesn't mix sanitized and plain object files.

## Clean

```sh
make clean
```

This removes the `.o` files, `treestats`, and `tests/test_bst`.

## Part 2: The crash

The test that triggers it: `test_empty_search`

The first line of the `make memcheck` report:

```sh
bst.c:111:17: runtime error: member access within null pointer of type 'struct node'
```

Why the original never noticed: The root itself is never checked if it's NULL but nodes are, so for a main that inserts values into the tree like the original did (there's no empty tree) the issue is never seen.

## Part 3: What order does to a tree

Keys 1 to 1023 in increasing order:

```sh
$ seq 1 1023 | ./treestats
n 1023, height 1023
```

Keys 1 to 1023 in a shuffled order:

```sh
$ seq 1 1023 | sort -R | ./treestats
n 1023, height 23
```

What insertion order does to the height, and number of visits a search can take: Insertion order affects the height because the first entry becomes the root and subsequent entries are further down the tree, so that an in-order insertion can result in a one-sided tree that's really like a linked list and a shuffled insert is more balanced. A search has a maximum equal to the depth of the tree, so insertion order also affects how long a search takes as is evident by the sorted vs shuffled tests above.

