#ifndef FSM_H
#define FSM_H

/*-----------------------------------------------------
* FSM module manages the 5 states, events (such as opening the door), and transitions.
* -----------------------------------------------------*/

typedef enum state {
    IDLE, PREWASH, WASH, DRY, DONE
} state_t;

#endif