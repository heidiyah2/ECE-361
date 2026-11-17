#ifndef FSM_H
#define FSM_H

/*-----------------------------------------------------
* FSM module manages the 5 states, events (such as opening the door), and transitions.
* -----------------------------------------------------*/

typedef enum states { IDLE, PREWASH, WASH, DRY, DONE, N_STATES } state_t;
typedef enum events { E_CLOSE, E_OPEN, N_EVENTS } event_t;
typedef enum actions { A_UNDEFINED, A_NONE, N_ACTIONS } action_t;

#endif
