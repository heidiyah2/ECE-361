## Copilot

I used Copilot for:

- Creating the module files.
- Filling header files with include guards.
- Adding the header-file includes to the C source files.
- Creating the Makefile to build and run the main program and regression test.
- Modifying the recursive maximum function so it only needs two inputs and no
  longer needs an `int i` counter.
- Writing `tests/test_stats.c`.

Issue: During the recursive-function work, Copilot initially tried to separate the recursive function into one function that supplied the `int i` argument and a recursive helper function. This overcomplicated the existing function. I corrected this by specifying that I wanted no external counter passed into the function and wanted the `arr` pointer incremented instead.

## Gemini

I used Gemini for general tips on how to refactor a program, reviewing scope, and summarizing common methods of incrementing recursive functions.

## Claudian

I used Claudian to help me understand what the bonus problem meant by "best value" and "tail call". 