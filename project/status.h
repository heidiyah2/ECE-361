#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>
#include <stdbool.h>
#include "fsm.h"

typedef struct status_fields {
    bool pump;
    bool heater;
    bool fan;
    bool paused;
    state_t state;
} status_fields_t;

/* Takes the fields in status_fields and writes the packed 32-bit word through *word; returns 0 on success. Returns -1 for NULL f or word, or invalid state. Assumes f points to filled struct. */
int status_pack(const status_fields_t *f, uint32_t *word);

/* Takes a 32-bit word and breaks it into fields stored in status_fields. Returns -1 for NULL struct or invalid word where reserved bits are not 0, and if state is invalid (does not unpack to f). */
int status_unpack(uint32_t word, status_fields_t *f);

#endif