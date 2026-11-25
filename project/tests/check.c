#include <stdio.h>
#include "check.h"

static int failed = 0;

void check(int ok, const char *what) {
    printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    fflush(stdout); /* so earlier results survive a crash */
    if (!ok) {
        failed++;
    }
}

int check_done(void) {
    return failed == 0 ? 0 : 1;
}
