## Copilot

I used Copilot for:

- Creating the module files.
- Filling header files with include guards.
- Adding the header-file includes to the C source files.
- Creating the Makefile to build and run the main program and regression test.
- Modifying the recursive maximum function so it only needs two inputs and no
  longer needs an `int i` counter.
- Writing `tests/test_stats.c`.
- Writing `depth.c`, specifically `main`.
- Writing `bonus.sh` to make analysis easier for the 4 test cases.

Issue: During the recursive-function work, Copilot initially tried to separate the recursive function into one function that supplied the `int i` argument and a recursive helper function. This overcomplicated the existing function. I corrected this by specifying that I wanted no external counter passed into the function and wanted the `arr` pointer incremented instead.

## Gemini

I used Gemini for general tips on how to refactor a program, reviewing scope, and summarizing common methods of incrementing recursive functions. Also to explore methods of programatically finding a limit, such as an exponential and binary search, and for understanding the `-O0` and `-O2` flags.

## Claudian

I used Claudian to help me understand what the bonus problem meant by "best value" and to decide whether the array of values should be stored on the stack or not.