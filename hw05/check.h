#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include "lock.h"

/* Checks table for problems starting from state start, prints one line per problem to out; entries left as A_UNDEFINED, next states out of range, states that cannot be reached from start (breadth-first), and states with no way out. Returns # of problems, or -1 if table or out is NULL or start is out of range. Assumes N_STATES and N_EVENTS are last in enums, out is open to write to, and bad entries aren't real (skips so if no arrow to a state it counts as unreachable). */
int fsm_check(const transition_t table[N_STATES][N_EVENTS], state_t start, FILE *out);

#endif
