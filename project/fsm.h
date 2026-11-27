#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "sensor.h"

/*-----------------------------------------------------
* FSM module manages the 5 states, events (such as opening the door), and transitions.
* -----------------------------------------------------*/

typedef enum states { IDLE, PREWASH, WASH, DRY, DONE, N_STATES } state_t;
typedef enum events { E_NONE, E_START_CLOSED, E_START_OPEN, E_DOOR_CLOSE, E_DOOR_OPEN, E_TIME_UP, E_DRY_ENOUGH, E_TEMP_LOW, E_TEMP_HIGH, N_EVENTS } event_t;
typedef enum actions { A_UNDEFINED, A_NONE, A_START_PAUSED, A_START_PREWASH, A_START_WASH, A_START_DRY, A_FINISH, A_PAUSE, A_RESUME, A_HEATER_ON, A_HEATER_OFF, N_ACTIONS } action_t;

/* Looks up next state and action from given state s and event e; returns next state and writes its action unless action is NULL. Out-of-range s or e returns IDLE with A_UNDEFINED. Assumes fsm_table is fully filled in, and that N_STATES and N_EVENTS come last in state_t and event_t. */
state_t next_state(state_t s, event_t e, action_t *action);

/* Everything the dishwasher remembers between ticks. */
typedef struct dishwasher {
    state_t state;
    bool pump;
    bool heater;
    bool fan;
    bool paused;
    bool heater_before_pause;
    time_t phase_start;
    time_t pause_start;
    uint32_t prev_switches;
} dishwasher_t;

/* IDLE, all outputs off, not paused, previous switches 0. Does nothing if d is NULL. */
void fsm_init(dishwasher_t *d);

/* Turns the reading and switch register into the one event for this tick.
 * Priority: door, START, time up, dry enough, temperature. Only door events while paused.
 * If the door closes on a paused phase whose time had already run out, returns E_TIME_UP.
 * Returns E_NONE if d or r is NULL. */
event_t fsm_event(const dishwasher_t *d, const reading_t *r, uint32_t switches);

/* One tick: makes the event, moves to the next state, saves the switches, applies the action.
 * Returns 0, or -1 if d or r is NULL (nothing changes), or on A_UNDEFINED or an invalid
 * state (then all outputs are off and the state is IDLE). */
int fsm_step(dishwasher_t *d, const reading_t *r, uint32_t switches);

/* RGB_LED_REG value for state s. */
uint32_t fsm_rgb(state_t s);

#endif
