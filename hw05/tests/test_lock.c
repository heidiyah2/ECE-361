#include "../lock.h"

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

// feed each character of keys through lock_event and lock_step, starting in
// LOCKED; characters that are not events are skipped. Returns the final state
// and writes the last action taken.
static state_t run(const char *keys, action_t *last) {
    state_t s = LOCKED;
    *last = A_UNDEFINED;
    for (const char *p = keys; *p != '\0'; p++) {
        int e = lock_event(s, *p);
        if (e >= 0) {
            s = lock_step(s, (event_t)e, last);
        }
    }
    return s;
}

// one expected table entry, written out by hand from the state diagram
typedef struct {
    state_t s;
    event_t e;
    state_t next;
    action_t action;
    const char *what;
} expected_t;

static void test_table_entries(void) {
    const expected_t want[] = {
        { LOCKED, E_RIGHT,   GOT1,   A_NONE,   "table: LOCKED + RIGHT -> GOT1 NONE" },
        { LOCKED, E_WRONG,   LOCKED, A_BEEP,   "table: LOCKED + WRONG -> LOCKED BEEP" },
        { LOCKED, E_CLOSE,   LOCKED, A_NONE,   "table: LOCKED + CLOSE -> LOCKED NONE" },
        { LOCKED, E_TIMEOUT, LOCKED, A_NONE,   "table: LOCKED + TIMEOUT -> LOCKED NONE" },
        { GOT1,   E_RIGHT,   GOT2,   A_NONE,   "table: GOT1 + RIGHT -> GOT2 NONE" },
        { GOT1,   E_WRONG,   LOCKED, A_BEEP,   "table: GOT1 + WRONG -> LOCKED BEEP" },
        { GOT1,   E_CLOSE,   GOT1,   A_NONE,   "table: GOT1 + CLOSE -> GOT1 NONE" },
        { GOT1,   E_TIMEOUT, LOCKED, A_NONE,   "table: GOT1 + TIMEOUT -> LOCKED NONE" },
        { GOT2,   E_RIGHT,   OPEN,   A_UNLOCK, "table: GOT2 + RIGHT -> OPEN UNLOCK" },
        { GOT2,   E_WRONG,   LOCKED, A_BEEP,   "table: GOT2 + WRONG -> LOCKED BEEP" },
        { GOT2,   E_CLOSE,   GOT2,   A_NONE,   "table: GOT2 + CLOSE -> GOT2 NONE" },
        { GOT2,   E_TIMEOUT, LOCKED, A_NONE,   "table: GOT2 + TIMEOUT -> LOCKED NONE" },
        { OPEN,   E_RIGHT,   OPEN,   A_NONE,   "table: OPEN + RIGHT -> OPEN NONE" },
        { OPEN,   E_WRONG,   OPEN,   A_NONE,   "table: OPEN + WRONG -> OPEN NONE" },
        { OPEN,   E_CLOSE,   LOCKED, A_LOCK,   "table: OPEN + CLOSE -> LOCKED LOCK" },
        { OPEN,   E_TIMEOUT, LOCKED, A_LOCK,   "table: OPEN + TIMEOUT -> LOCKED LOCK" },
    };
    int n = sizeof want / sizeof want[0];
    for (int i = 0; i < n; i++) {
        action_t a = A_UNDEFINED;
        state_t next = lock_step(want[i].s, want[i].e, &a);
        check(next == want[i].next && a == want[i].action, want[i].what);
    }
}

static void test_table_complete(void) {
    int none_undefined = 1;
    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            if (lock_table[s][e].action == A_UNDEFINED) {
                none_undefined = 0;
            }
        }
    }
    check(none_undefined, "table: no entry left as A_UNDEFINED");
}

static void test_step_bad_input(void) {
    action_t a = A_NONE;
    check(lock_step(N_STATES, E_RIGHT, &a) == LOCKED && a == A_UNDEFINED,
          "step: state out of range -> LOCKED UNDEFINED");
    a = A_NONE;
    check(lock_step(GOT1, N_EVENTS, &a) == LOCKED && a == A_UNDEFINED,
          "step: event out of range -> LOCKED UNDEFINED");
    a = A_NONE;
    check(lock_step(GOT1, (event_t)-1, &a) == LOCKED && a == A_UNDEFINED,
          "step: event -1 -> LOCKED UNDEFINED");
    check(lock_step(GOT2, E_RIGHT, NULL) == OPEN,
          "step: NULL action still returns next state");
    check(lock_step(N_STATES, E_RIGHT, NULL) == LOCKED,
          "step: NULL action and bad state -> LOCKED");
}

static void test_event_digits(void) {
    check(lock_event(LOCKED, '3') == E_RIGHT, "event: LOCKED '3' is RIGHT");
    check(lock_event(LOCKED, '1') == E_WRONG, "event: LOCKED '1' is WRONG");
    check(lock_event(GOT1, '1') == E_RIGHT, "event: GOT1 '1' is RIGHT");
    check(lock_event(GOT1, '3') == E_WRONG, "event: GOT1 '3' is WRONG");
    check(lock_event(GOT2, '4') == E_RIGHT, "event: GOT2 '4' is RIGHT");
    check(lock_event(GOT2, '9') == E_WRONG, "event: GOT2 '9' is WRONG");
    check(lock_event(OPEN, '3') == E_WRONG, "event: OPEN '3' is WRONG (no next digit)");
    check(lock_event(OPEN, '0') == E_WRONG, "event: OPEN '0' is WRONG");
}

static void test_event_other_chars(void) {
    int close_all = 1;
    int timeout_all = 1;
    for (int s = 0; s < N_STATES; s++) {
        if (lock_event(s, 'c') != E_CLOSE) {
            close_all = 0;
        }
        if (lock_event(s, 't') != E_TIMEOUT) {
            timeout_all = 0;
        }
    }
    check(close_all, "event: 'c' is CLOSE in every state");
    check(timeout_all, "event: 't' is TIMEOUT in every state");
    check(lock_event(LOCKED, 'x') == -1, "event: 'x' is -1");
    check(lock_event(LOCKED, '\n') == -1, "event: newline is -1");
    check(lock_event(LOCKED, ' ') == -1, "event: space is -1");
    check(lock_event(LOCKED, 'C') == -1, "event: uppercase 'C' is -1");
    check(lock_event(N_STATES, '3') == -1, "event: state out of range is -1");
}

static void test_sequences(void) {
    action_t a;
    check(run("314", &a) == OPEN && a == A_UNLOCK, "sequence: 3 1 4 opens");
    check(run("3314", &a) == LOCKED, "sequence: 3 3 1 4 does not open");
    check(run("31t4", &a) == LOCKED, "sequence: 3 1 t 4 does not open");
    check(run("314c", &a) == LOCKED && a == A_LOCK, "sequence: 3 1 4 c locks again");
    check(run("3 1\n4", &a) == OPEN, "sequence: spaces and newlines are skipped");
}

int main(void) {
    test_table_entries();
    test_table_complete();
    test_step_bad_input();
    test_event_digits();
    test_event_other_chars();
    test_sequences();

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
