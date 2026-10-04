#ifndef STATUS_H
#define STATUS_H

#include "bits.h"
#include <stdbool.h>

typedef struct {
    bool STATUS_HEAT; // true if set
    bool STATUS_COOL;
    bool STATUS_FAN;
    bool STATUS_FAULT;
    enum status_mode {
        STATUS_MODE_INVALID = -1,
        STATUS_MODE_OFF = 0,
        STATUS_MODE_HEAT = 1,
        STATUS_MODE_COOL = 2,
        STATUS_MODE_AUTO = 3,
        STATUS_MODE_FAN_ONLY = 4
    } STATUS_MODE;
    bool STATUS_RESERVED;
    int8_t STATUS_SETPOINT;
} status_t;

status_t status_unpack(uint16_t word);
void status_print(status_t status);

#endif