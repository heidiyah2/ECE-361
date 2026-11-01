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
