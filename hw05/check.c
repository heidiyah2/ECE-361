#include "check.h"

int fsm_check(const transition_t table[N_STATES][N_EVENTS], state_t start, FILE *out) {
    if (table == NULL || start >= N_STATES || start < 0 || out == NULL) {
        return -1;
    }
    int problems = 0;

    /* Find missing entries and bad entries */
    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            transition_t t = table[s][e];
            if (t.action == A_UNDEFINED) {
                fprintf(out, "missing: %s %s\n", state_name(s), event_name(e));
                problems++;
            }
            if (t.next >= N_STATES || t.next < 0) {
                fprintf(out, "invalid next state: %s %s\n", state_name(s), event_name(e));
                problems++;
            }
        }
    }
    /* Find which states can be reached */
    int visited[N_STATES] = {0}, queue[N_STATES];
    int head = 0, tail = 0;

    visited[start] = 1;
    queue[tail++] = start;
    while (head < tail) {
        int s = queue[head++];
        for (int e = 0; e < N_EVENTS; e++) {
            transition_t t = table[s][e];
            /* skip entries that are not real transitions */
            if (t.action == A_UNDEFINED || t.next >= N_STATES || t.next < 0) {
                continue;
            }
            if (!visited[t.next]) {
                visited[t.next] = 1;
                queue[tail++] = t.next;
            }
        }
    }
    for (int s = 0; s < N_STATES; s++) {
        if (!visited[s]) {
            fprintf(out, "unreachable: %s\n", state_name(s));
            problems++;
        }
    }

    /* Find states with no way out (dead ends) */
    for (int s = 0; s < N_STATES; s++) {
        int has_exit = 0;
        for (int e = 0; e < N_EVENTS; e++) {
            transition_t t = table[s][e];
            /* missing or out-of-range entries are not a way out */
            if (t.action == A_UNDEFINED || t.next >= N_STATES || t.next < 0) {
                continue;
            }
            if (t.next != (state_t)s) {
                has_exit = 1;
            }
        }
        if (!has_exit) {
            fprintf(out, "no way out: %s\n", state_name(s));
            problems++;
        }
    }

    return problems;
}
