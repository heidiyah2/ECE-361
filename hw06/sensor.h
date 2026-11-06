#ifndef SENSOR_H
#define SENSOR_H

#include <time.h>
#include "iom361_r4.h"

typedef struct reading {
    time_t t;
    float temp_c;
    float rh_pct;
} reading_t;

/* Reads TEMP_REG and HUMID_REG, keeps bits 19..0 of each, converts them with the formula in iom361_r4.h, rounds to 0.1 and stores them with the timestamp now in *r. Returns 0, or the iom361 return code (rc) of a failed read; then changes nothing. Assumes base and r are non-NULL, and r exists. */
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif
