#include "fsm.h"

/* One cell of the transition table: where to go, and what to do on the way. */
typedef struct transition {
    state_t next_state;
    action_t action;
} transition_t;

/* Transition table: row = current state, column = event.
 * Each cell is { next_state, action }. A cell left out would be
 * { IDLE, A_UNDEFINED }, so every cell is written out explicitly. */
static const transition_t fsm_table[N_STATES][N_EVENTS] = {
    [IDLE] = {
        [E_NONE]         = { IDLE, A_NONE },
        [E_START_CLOSED] = { PREWASH, A_START_PREWASH },
        [E_START_OPEN]   = { PREWASH, A_START_PAUSED },
        [E_DOOR_CLOSE]   = { IDLE, A_NONE },
        [E_DOOR_OPEN]    = { IDLE, A_NONE },
        [E_TIME_UP]      = { IDLE, A_NONE },
        [E_DRY_ENOUGH]   = { IDLE, A_NONE },
        [E_TEMP_LOW]     = { IDLE, A_NONE },
        [E_TEMP_HIGH]    = { IDLE, A_NONE },
    },
    [PREWASH] = {
        [E_NONE]         = { PREWASH, A_NONE },
        [E_START_CLOSED] = { PREWASH, A_NONE },
        [E_START_OPEN]   = { PREWASH, A_NONE },
        [E_DOOR_CLOSE]   = { PREWASH, A_RESUME },
        [E_DOOR_OPEN]    = { PREWASH, A_PAUSE }, 
        [E_TIME_UP]      = { WASH, A_START_WASH },     /* 90 ticks */
        [E_DRY_ENOUGH]   = { PREWASH, A_NONE },
        [E_TEMP_LOW]     = { PREWASH, A_NONE },
        [E_TEMP_HIGH]    = { PREWASH, A_NONE },
    },
    [WASH] = {
        [E_NONE]         = { WASH, A_NONE },
        [E_START_CLOSED] = { WASH, A_NONE },
        [E_START_OPEN]   = { WASH, A_NONE },
        [E_DOOR_CLOSE]   = { WASH, A_RESUME },
        [E_DOOR_OPEN]    = { WASH, A_PAUSE },
        [E_TIME_UP]      = { DRY, A_START_DRY },       /* 240 ticks */
        [E_DRY_ENOUGH]   = { WASH, A_NONE },
        [E_TEMP_LOW]     = { WASH, A_HEATER_ON },      /* below 53.0 C */
        [E_TEMP_HIGH]    = { WASH, A_HEATER_OFF },     /* at 55.0 C */
    },
    [DRY] = {
        [E_NONE]         = { DRY, A_NONE },
        [E_START_CLOSED] = { DRY, A_NONE },
        [E_START_OPEN]   = { DRY, A_NONE },
        [E_DOOR_CLOSE]   = { DRY, A_RESUME },
        [E_DOOR_OPEN]    = { DRY, A_PAUSE },
        [E_TIME_UP]      = { DONE, A_FINISH },         /* 600 ticks max */
        [E_DRY_ENOUGH]   = { DONE, A_FINISH },         /* below 40 %RH */
        [E_TEMP_LOW]     = { DRY, A_NONE },
        [E_TEMP_HIGH]    = { DRY, A_NONE },
    },
    [DONE] = {
        [E_NONE]         = { DONE, A_NONE },
        [E_START_CLOSED] = { DONE, A_NONE },           /* must open door first */
        [E_START_OPEN]   = { DONE, A_NONE },
        [E_DOOR_CLOSE]   = { DONE, A_NONE },
        [E_DOOR_OPEN]    = { IDLE, A_NONE },
        [E_TIME_UP]      = { DONE, A_NONE },
        [E_DRY_ENOUGH]   = { DONE, A_NONE },
        [E_TEMP_LOW]     = { DONE, A_NONE },
        [E_TEMP_HIGH]    = { DONE, A_NONE },
    },
};
