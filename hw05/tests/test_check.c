#include "../check.h"
#include <string.h>

static int passed = 0;
static int failed = 0;

// print PASS or FAIL for one check and count it
static void check(int ok, const char *what) {
    printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (ok) {
        passed++;
    } else {
        failed++;
    }
}

// run fsm_check on table into a temporary stream; copies what it printed
// into buf and writes the number of lines printed. Returns fsm_check's result.
static int run_check(const transition_t table[N_STATES][N_EVENTS], state_t start,
                     char *buf, size_t size, int *lines) {
    buf[0] = '\0';
    *lines = 0;
    FILE *f = tmpfile();
    if (f == NULL) {
        return -2;
    }
    int result = fsm_check(table, start, f);
    rewind(f);
    size_t n = fread(buf, 1, size - 1, f);
    buf[n] = '\0';
    fclose(f);
    for (size_t i = 0; i < n; i++) {
        if (buf[i] == '\n') {
            (*lines)++;
        }
    }
    return result;
}

// start each broken table as a copy of the real one
static void copy_table(transition_t t[N_STATES][N_EVENTS]) {
    memcpy(t, lock_table, sizeof(transition_t) * N_STATES * N_EVENTS);
}

static void test_real_table(void) {
    char buf[1024];
    int lines;
    int n = run_check(lock_table, LOCKED, buf, sizeof buf, &lines);
    check(n == 0, "real table: 0 problems");
    check(lines == 0, "real table: prints nothing");
}

static void test_missing_entry(void) {
    transition_t t[N_STATES][N_EVENTS];
    char buf[1024];
    int lines;
    copy_table(t);
    t[LOCKED][E_CLOSE] = (transition_t){0};
    int n = run_check(t, LOCKED, buf, sizeof buf, &lines);
    check(n == 1, "missing entry: 1 problem");
    check(strstr(buf, "missing: LOCKED CLOSE") != NULL, "missing entry: reported");
    check(lines == n, "missing entry: one line per problem");
}

static void test_bad_next(void) {
    transition_t t[N_STATES][N_EVENTS];
    char buf[1024];
    int lines;
    copy_table(t);
    t[OPEN][E_CLOSE].next = (state_t)7;
    int n = run_check(t, LOCKED, buf, sizeof buf, &lines);
    check(n == 1, "bad next: 1 problem");
    check(strstr(buf, "invalid next state: OPEN CLOSE") != NULL, "bad next: reported");
    check(lines == n, "bad next: one line per problem");
}

static void test_unreachable(void) {
    transition_t t[N_STATES][N_EVENTS];
    char buf[1024];
    int lines;
    copy_table(t);
    t[GOT2][E_RIGHT].next = LOCKED;
    int n = run_check(t, LOCKED, buf, sizeof buf, &lines);
    check(n == 1, "unreachable: 1 problem");
    check(strstr(buf, "unreachable: OPEN") != NULL, "unreachable: OPEN reported");
    check(lines == n, "unreachable: one line per problem");
}

static void test_no_way_out(void) {
    transition_t t[N_STATES][N_EVENTS];
    char buf[1024];
    int lines;
    copy_table(t);
    t[OPEN][E_CLOSE].next = OPEN;
    t[OPEN][E_TIMEOUT].next = OPEN;
    int n = run_check(t, LOCKED, buf, sizeof buf, &lines);
    check(n == 1, "no way out: 1 problem");
    check(strstr(buf, "no way out: OPEN") != NULL, "no way out: OPEN reported");
    check(lines == n, "no way out: one line per problem");
}

static void test_unreachable_and_no_way_out(void) {
    transition_t t[N_STATES][N_EVENTS];
    char buf[1024];
    int lines;
    copy_table(t);
    t[GOT2][E_RIGHT].next = LOCKED;
    t[OPEN][E_CLOSE].next = OPEN;
    t[OPEN][E_TIMEOUT].next = OPEN;
    int n = run_check(t, LOCKED, buf, sizeof buf, &lines);
    check(n == 2, "both: 2 problems");
    check(strstr(buf, "unreachable: OPEN") != NULL, "both: unreachable reported");
    check(strstr(buf, "no way out: OPEN") != NULL, "both: no way out reported");
    check(lines == n, "both: one line per problem");
}

static void test_missing_causes_unreachable(void) {
    transition_t t[N_STATES][N_EVENTS];
    char buf[1024];
    int lines;
    copy_table(t);
    t[GOT2][E_RIGHT] = (transition_t){0};
    int n = run_check(t, LOCKED, buf, sizeof buf, &lines);
    check(n == 2, "missing GOT2 RIGHT: 2 problems");
    check(strstr(buf, "missing: GOT2 RIGHT") != NULL, "missing GOT2 RIGHT: missing reported");
    check(strstr(buf, "unreachable: OPEN") != NULL, "missing GOT2 RIGHT: OPEN unreachable");
    check(lines == n, "missing GOT2 RIGHT: one line per problem");
}

static void test_all_zero(void) {
    transition_t t[N_STATES][N_EVENTS] = {0};
    char buf[2048];
    int lines;
    int n = run_check(t, LOCKED, buf, sizeof buf, &lines);
    check(n == 23, "all zero: 23 problems (16 missing, 3 unreachable, 4 no way out)");
    check(lines == n, "all zero: one line per problem");
}

static void test_other_start(void) {
    char buf[1024];
    int lines;
    int n = run_check(lock_table, OPEN, buf, sizeof buf, &lines);
    check(n == 0, "start OPEN: real table has 0 problems");
    check(lines == 0, "start OPEN: prints nothing");
}

static void test_bad_args(void) {
    char buf[1024];
    int lines;
    check(fsm_check(NULL, LOCKED, stdout) == -1, "bad args: NULL table returns -1");
    check(fsm_check(lock_table, LOCKED, NULL) == -1, "bad args: NULL out returns -1");
    int n = run_check(lock_table, N_STATES, buf, sizeof buf, &lines);
    check(n == -1, "bad args: start out of range returns -1");
    check(lines == 0, "bad args: start out of range prints nothing");
}

int main(void) {
    test_real_table();
    test_missing_entry();
    test_bad_next();
    test_unreachable();
    test_no_way_out();
    test_unreachable_and_no_way_out();
    test_missing_causes_unreachable();
    test_all_zero();
    test_other_start();
    test_bad_args();

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
