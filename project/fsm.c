#include <stddef.h>
#include "fsm.h"

/* One cell of the transition table: where to go, and what to do on the way. */
typedef struct transition {
    state_t next;
    action_t action;
} transition_t;

/* Transition table: row = current state, column = event.
 * Each cell is { next_state, action }. A cell left out would be { IDLE, A_UNDEFINED }. */
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

state_t next_state(state_t s, event_t e, action_t *action) {
    if (s >= N_STATES || e >= N_EVENTS || s < 0 || e < 0) {
        if (action != NULL)
            *action = A_UNDEFINED;
        return IDLE;
    }
    transition_t t = fsm_table[s][e];

    if (action == NULL) {
        return t.next;
    }
    *action = t.action;
    return t.next;
}

#define SW_START        0x1u    // mask for bit 0
#define SW_DOOR_CLOSED  0x2u    // bit 1

#define HEATER_ON_BELOW_C   53.0f
#define HEATER_OFF_AT_C     55.0f
#define DRY_BELOW_RH        40.0f

static const time_t phase_ticks[N_STATES] = {
    [PREWASH] = 90,
    [WASH]    = 240,
    [DRY]     = 600,
};

void fsm_init(dishwasher_t *d) {
    if (d == NULL)
        return;
    *d = (dishwasher_t){ 0 };   // compound literal { 0 } sets every field to zero (members not listed set to 0)
    d->state = IDLE;
}

// True if the phase has used up its time; time spent paused does not count
static bool phase_over(const dishwasher_t *d, time_t now) {
    if (d->state < 0 || d->state >= N_STATES)
        return false;
    time_t limit = phase_ticks[d->state];
    time_t end = d->paused ? d->pause_start : now;
    return limit > 0 && end - d->phase_start >= limit;
}

event_t fsm_event(const dishwasher_t *d, const reading_t *r, uint32_t switches) {
    if (d == NULL || r == NULL)
        return E_NONE;
    bool door_changed = ((switches ^ d->prev_switches) & SW_DOOR_CLOSED) != 0;
    bool door_closed = (switches & SW_DOOR_CLOSED) != 0;
    bool start_pressed = (switches & SW_START) && !(d->prev_switches & SW_START);

    if (door_changed) {
        if (door_closed && d->paused && phase_over(d, r->timestamp))    // check if door closed on final phase tick
            return E_TIME_UP;
        return door_closed ? E_DOOR_CLOSE : E_DOOR_OPEN;
    }
    if (d->paused)
        return E_NONE;
    if (start_pressed)
        return door_closed ? E_START_CLOSED : E_START_OPEN;
    if (phase_over(d, r->timestamp))
        return E_TIME_UP;
    if (d->state == DRY && (r->rh_pct < DRY_BELOW_RH))
        return E_DRY_ENOUGH;
    if (r->temp_c < HEATER_ON_BELOW_C)
        return E_TEMP_LOW;
    if (r->temp_c >= HEATER_OFF_AT_C)
        return E_TEMP_HIGH;
    return E_NONE;
}

// Helper for apply_action cases
static void all_off(dishwasher_t *d) {
    d->pump = false;
    d->heater = false;
    d->fan = false;
}

static int apply_action(dishwasher_t *d, action_t a, const reading_t *r) {
    switch (a) {
    case A_NONE:
        break;
    case A_START_PREWASH:
        all_off(d);
        d->paused = false;
        d->pump = true;
        d->phase_start = r->timestamp;
        break;
    case A_START_PAUSED:
        all_off(d);
        d->paused = true;
        d->heater_before_pause = false;
        d->phase_start = r->timestamp;
        d->pause_start = r->timestamp;
        break;
    case A_START_WASH:
        all_off(d);
        d->paused = false;
        d->pump = true;
        d->heater = r->temp_c < HEATER_ON_BELOW_C;
        d->phase_start = r->timestamp;
        break;
    case A_START_DRY:
        all_off(d);
        d->paused = false;
        d->fan = true;
        d->phase_start = r->timestamp;
        break;
    case A_FINISH:
        all_off(d);
        d->paused = false;
        break;
    case A_PAUSE:
        d->heater_before_pause = d->heater;
        all_off(d);
        d->paused = true;
        d->pause_start = r->timestamp;
        break;
    case A_RESUME:
        if (!d->paused)
            break;
        d->paused = false;
        d->phase_start += r->timestamp - d->pause_start;    // offset phase start by pause time
        d->pump = d->state == PREWASH || d->state == WASH;  // pump on if state is PREWASH or WASH
        d->fan = d->state == DRY;                           // fan on if state is DRY
        d->heater = d->state == WASH &&                     // heater on according to hysteresis in WASH state
                    (d->heater_before_pause || r->temp_c < HEATER_ON_BELOW_C);
        break;
    case A_HEATER_ON:
        d->heater = true;
        break;
    case A_HEATER_OFF:
        d->heater = false;
        break;
    default:
        all_off(d);         // if table is incomplete turn everything off, return to IDLE, return error code for main to handle
        d->paused = false;
        d->state = IDLE;
        return -1;
    }
    return 0;
}

// each tick:
int fsm_step(dishwasher_t *d, const reading_t *r, uint32_t switches) {
    if (d == NULL || r == NULL)
        return -1;
    action_t a;
    event_t e = fsm_event(d, r, switches);  // d->prev_switches still in place, can compare with switches

    d->state = next_state(d->state, e, &a);
    d->prev_switches = switches;
    return apply_action(d, a, r);
}

uint32_t fsm_rgb(state_t s) {
    switch (s) {
    case PREWASH: return build_rgb_reg(1, 255, 255, 255);   // White
    case WASH:    return build_rgb_reg(1, 0, 0, 255);       // Blue
    case DRY:     return build_rgb_reg(1, 255, 255, 0);     // Yellow
    case DONE:    return build_rgb_reg(1, 0, 255, 0);       // Green
    default:      return build_rgb_reg(1, 0, 0, 0);         // Black (off)
    }
}
