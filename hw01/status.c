#include "status.h"
#include <stdio.h>

enum {
    STATUS_HEAT_POS = 0,
    STATUS_HEAT_WIDTH = 1,
    STATUS_COOL_POS = 1,
    STATUS_COOL_WIDTH = 1,
    STATUS_FAN_POS = 2,
    STATUS_FAN_WIDTH = 1,
    STATUS_FAULT_POS = 3,
    STATUS_FAULT_WIDTH = 1,
    STATUS_MODE_POS = 4,
    STATUS_MODE_WIDTH = 3,
    STATUS_RESERVED_POS = 7,
    STATUS_RESERVED_WIDTH = 1,
    STATUS_SETPOINT_POS = 8,
    STATUS_SETPOINT_WIDTH = 8
};

status_t status_unpack(uint16_t word)
{
    status_t status;
    uint32_t mode;

    status.STATUS_HEAT = get_field(word, STATUS_HEAT_POS, STATUS_HEAT_WIDTH) != 0;
    status.STATUS_COOL = get_field(word, STATUS_COOL_POS, STATUS_COOL_WIDTH) != 0;
    status.STATUS_FAN = get_field(word, STATUS_FAN_POS, STATUS_FAN_WIDTH) != 0;
    status.STATUS_FAULT = get_field(word, STATUS_FAULT_POS, STATUS_FAULT_WIDTH) != 0;

    mode = get_field(word, STATUS_MODE_POS, STATUS_MODE_WIDTH);
    status.STATUS_MODE = mode <= (uint32_t)STATUS_MODE_FAN_ONLY
        ? (enum status_mode)mode //matches enum mode if valid
        : STATUS_MODE_INVALID;

    status.STATUS_RESERVED = get_field(word, STATUS_RESERVED_POS, STATUS_RESERVED_WIDTH) != 0;
    status.STATUS_SETPOINT = (int8_t)sign_extend(
        get_field(word, STATUS_SETPOINT_POS, STATUS_SETPOINT_WIDTH),
        STATUS_SETPOINT_WIDTH);

    return status;
}

static const char *status_mode_name(enum status_mode mode) // pointer so prinf can use
{
    switch (mode) {
    case STATUS_MODE_OFF:
        return "OFF";
    case STATUS_MODE_HEAT:
        return "HEAT";
    case STATUS_MODE_COOL:
        return "COOL";
    case STATUS_MODE_AUTO:
        return "AUTO";
    case STATUS_MODE_FAN_ONLY:
        return "FAN_ONLY";
    case STATUS_MODE_INVALID:
        return "INVALID";
    }

    return "INVALID";
}

void status_print(status_t status)
{
    printf("Heater: %s\n", status.STATUS_HEAT ? "on" : "off");
    printf("Compressor: %s\n", status.STATUS_COOL ? "on" : "off");
    printf("Fan: %s\n", status.STATUS_FAN ? "on" : "off");
    printf("Fault: %s\n", status.STATUS_FAULT ? "fault detected" : "no fault");
    printf("Mode: %d (%s)\n", (int)status.STATUS_MODE,
        status_mode_name(status.STATUS_MODE));
    printf("Reserved bit: %s\n", status.STATUS_RESERVED ? "set" : "clear");
    printf("Setpoint: %d\xC2\xB0" "C\n", (int)status.STATUS_SETPOINT);
}