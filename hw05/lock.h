#ifndef LOCK_H
#define LOCK_H

#include <stdio.h>

typedef enum { LOCKED, GOT1, GOT2, OPEN, N_STATES } state_t;
typedef enum { E_RIGHT, E_WRONG, E_CLOSE, E_TIMEOUT, N_EVENTS } event_t;
typedef enum { A_UNDEFINED, A_NONE, A_UNLOCK, A_LOCK, A_BEEP, N_ACTIONS } action_t;
typedef struct { state_t next; action_t action; } transition_t;

extern const transition_t lock_table[N_STATES][N_EVENTS];

/* Looks up next state and action from given state s and event e; returns next state and writes its action unless action is NULL. Out-of-range s or e returns LOCKED with A_UNDEFINED. Assumes lock_table is fully filled in. */
state_t lock_step(state_t s, event_t e, action_t *action);

/* Turns a character into an event and returns it. Returns -1 otherwise. */
int lock_event(state_t s, char c);

const char *state_name(state_t s);
const char *event_name(event_t e);
const char *action_name(action_t a);

#endif
