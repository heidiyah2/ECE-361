#include "lock.h"
#include <stdio.h>

int main(void) {
    state_t state = LOCKED, next_state;
    action_t action;
    int ch, event;

    while ((ch = getchar()) != EOF) {
        event = lock_event(state, (char)ch);
        if (event == -1) {
            continue;
        }
        next_state = lock_step(state, event, &action);
        printf("%s %c %s -> %s %s\n", state_name(state), ch, event_name(event), state_name(next_state), action_name(action));
        state = next_state;
    }
    printf("final: %s\n", state_name(state));

    return 0;
}
