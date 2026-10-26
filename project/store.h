#ifndef STORE_H
#define STORE_H

#include "sensor.h"
#include "fsm.h"

/*-----------------------------------------------------
* Store module handles readings, current state, and status word.
* -----------------------------------------------------*/

typedef struct record {
    reading_t reading;
    state_t current_state;
    uint32_t status_word;
} record_t;

typedef struct store store_t;

#endif