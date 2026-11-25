#ifndef FSM_H
#define FSM_H

/*-----------------------------------------------------
* FSM module manages the 5 states, events (such as opening the door), and transitions.
* -----------------------------------------------------*/

typedef enum states { IDLE, PREWASH, WASH, DRY, DONE, N_STATES } state_t;
typedef enum events { E_NONE, E_START_CLOSED, E_START_OPEN, E_DOOR_CLOSE, E_DOOR_OPEN, E_TIME_UP, E_DRY_ENOUGH, E_TEMP_LOW, E_TEMP_HIGH, N_EVENTS } event_t;
typedef enum actions { A_UNDEFINED, A_NONE, A_START_PAUSED, A_START_PREWASH, A_START_WASH, A_START_DRY, A_FINISH, A_PAUSE, A_RESUME, A_HEATER_ON, A_HEATER_OFF, N_ACTIONS } action_t;

/* Looks up next state and action from given state s and event e; returns next state and writes its action unless action is NULL. Out-of-range s or e returns IDLE with A_UNDEFINED. Assumes fsm_table is fully filled in, and that N_STATES and N_EVENTS come last in state_t and event_t. */
state_t next_state(state_t s, event_t e, action_t *action);

#endif
