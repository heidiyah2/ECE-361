#include <stddef.h>
#include "status.h"

enum {
    STATUS_PUMP_POS = 0,
    STATUS_PUMP_WIDTH = 1,
    STATUS_HEATER_POS = 1,
    STATUS_HEATER_WIDTH = 1,
    STATUS_FAN_POS = 2,
    STATUS_FAN_WIDTH = 1,
    STATUS_PAUSED_POS = 3,
    STATUS_PAUSED_WIDTH = 1,
    STATUS_STATE_POS = 4,
    STATUS_STATE_WIDTH = 3
};

static uint32_t get_field(uint32_t word, int pos, int width) 
{
    uint32_t field, mask;

    if (width < 1 || width > 32 || pos < 0 || pos > 31 || pos + width > 32)
        field = UINT32_MAX;
    else {
        mask = width == 32 ? UINT32_MAX : (UINT32_C(1) << width) - 1u; // condition ? value_if_true : value_if_false
        field = (word >> pos) & mask;
    }

    return field;
}

static uint32_t set_field(uint32_t word, int pos, int width, uint32_t value)
{
    uint32_t mask, clear, set;

    if (width < 1 || width > 32 || pos < 0 || pos > 31 || pos + width > 32)
        word = 0xFFFFFFFF;
    else {
        mask = width == 32 ? UINT32_MAX : (UINT32_C(1) << width) - 1u;
        clear = word & ~(mask << pos);
        set = (value &= mask ) << pos;
        word = clear | set;
    }

    return word;
}

int status_pack(const status_fields_t *f, uint32_t *word) {
    if (f == NULL || word == NULL || f->state >= N_STATES)
        return -1;
    // start with blank word
    *word = 0;
    // pack each field
    *word = set_field(*word, STATUS_PUMP_POS, STATUS_PUMP_WIDTH, f->pump);
    *word = set_field(*word, STATUS_HEATER_POS, STATUS_HEATER_WIDTH, f->heater);
    *word = set_field(*word, STATUS_FAN_POS, STATUS_FAN_WIDTH, f->fan);
    *word = set_field(*word, STATUS_PAUSED_POS, STATUS_PAUSED_WIDTH, f->paused);
    *word = set_field(*word, STATUS_STATE_POS, STATUS_STATE_WIDTH, f->state);
    
    return 0;
}

int status_unpack(uint32_t word, status_fields_t *f) {
    if (f == NULL)
        return -1;
    // check reserved bits are 0
    uint32_t mask = (UINT32_C(1) << (STATUS_STATE_POS + STATUS_STATE_WIDTH)) - 1u;
    if ((word & ~mask) != 0)
        return -1;
    // read state
    state_t temp_state = (state_t)get_field(word, STATUS_STATE_POS, STATUS_STATE_WIDTH);
    if (temp_state >= N_STATES)     // no unpacking if invalid
        return -1;
    // unpack all fields
    status_fields_t temp;
    temp.pump = get_field(word, STATUS_PUMP_POS, STATUS_PUMP_WIDTH);    // any nonzero value becomes true (uint32 -> bool)
    temp.heater = get_field(word, STATUS_HEATER_POS, STATUS_HEATER_WIDTH);
    temp.fan = get_field(word, STATUS_FAN_POS, STATUS_FAN_WIDTH);
    temp.paused = get_field(word, STATUS_PAUSED_POS, STATUS_PAUSED_WIDTH);
    temp.state = temp_state;
    // copy to f
    *f = temp;
    return 0;
}
