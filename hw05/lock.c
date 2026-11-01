#include "lock.h"

const transition_t lock_table[N_STATES][N_EVENTS] = {
    [LOCKED] = {
        [E_RIGHT]   = { .next = GOT1,    .action = A_NONE },
        [E_WRONG]   = { .next = LOCKED,  .action = A_BEEP },
        [E_CLOSE]   = { .next = LOCKED,  .action = A_NONE },
        [E_TIMEOUT] = { .next = LOCKED,  .action = A_NONE }
    },
    [GOT1] = {
        [E_RIGHT]   = { .next = GOT2,    .action = A_NONE },
        [E_WRONG]   = { .next = LOCKED,  .action = A_BEEP },
        [E_CLOSE]   = { .next = GOT1,    .action = A_NONE },
        [E_TIMEOUT] = { .next = LOCKED,  .action = A_NONE }
    },
    [GOT2] = {
        [E_RIGHT]   = { .next = OPEN,    .action = A_UNLOCK },
        [E_WRONG]   = { .next = LOCKED,  .action = A_BEEP },
        [E_CLOSE]   = { .next = GOT2,    .action = A_NONE },
        [E_TIMEOUT] = { .next = LOCKED,  .action = A_NONE }
    },
    [OPEN] = {
        [E_RIGHT]   = { .next = OPEN,    .action = A_NONE },
        [E_WRONG]   = { .next = OPEN,    .action = A_NONE },
        [E_CLOSE]   = { .next = LOCKED,  .action = A_LOCK },
        [E_TIMEOUT] = { .next = LOCKED,  .action = A_LOCK }
    }
};

static const char code[] = "314";
static const char *state_names[N_STATES] = {
    [LOCKED] = "LOCKED",
    [GOT1]   = "GOT1",
    [GOT2]   = "GOT2",
    [OPEN]   = "OPEN"
};
static const char *event_names[N_EVENTS] = {
    [E_RIGHT]   = "RIGHT",
    [E_WRONG]   = "WRONG",
    [E_CLOSE]   = "CLOSE",
    [E_TIMEOUT] = "TIMEOUT"
};
static const char *action_names[N_ACTIONS] = {
    [A_UNDEFINED] = "UNDEFINED",
    [A_NONE]      = "NONE",
    [A_UNLOCK]    = "UNLOCK",
    [A_LOCK]      = "LOCK",
    [A_BEEP]      = "BEEP"
};

state_t lock_step(state_t s, event_t e, action_t *action) {
    if (s >= N_STATES || e >= N_EVENTS || s < 0 || e < 0) {
        if (action != NULL) {
            *action = A_UNDEFINED;
        }
        return LOCKED;
    }

    transition_t t = lock_table[s][e];

    if (action == NULL) {
        return t.next;
    }

    *action = t.action;
    return t.next;
}

int lock_event(state_t s, char c) {
    if (s >= N_STATES || s < 0) {
        return -1;
    }
    if (c == 'c') {
        return E_CLOSE;
    }
    if (c == 't') {
        return E_TIMEOUT;
    }
    if (c >= '0' && c <= '9') {
        return c == code[s] ? E_RIGHT : E_WRONG;
    }
    return -1;
}

const char *state_name(state_t s) {
    if (s >= N_STATES || s < 0) {
        return "?";
    }
    return state_names[s];
}

const char *event_name(event_t e) {
    if (e >= N_EVENTS || e < 0) {
        return "?";
    }
    return event_names[e];
}

const char *action_name(action_t a) {
    if (a >= N_ACTIONS || a < 0) {
        return "?";
    }
    return action_names[a];
}
