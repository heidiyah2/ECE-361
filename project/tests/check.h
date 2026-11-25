#ifndef CHECK_H
#define CHECK_H

/*-----------------------------------------------------
* Shared test helper. Every unit test reports through check(), so every
* program prints results the same way for tests/run_tests.sh to count.
* -----------------------------------------------------*/

/* Prints "PASS: what" if ok is nonzero, otherwise "FAIL: what", and counts it. */
void check(int ok, const char *what);

/* Returns the exit status for main: 0 if no check failed, 1 otherwise. */
int check_done(void);

#endif
