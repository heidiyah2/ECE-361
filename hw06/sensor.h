#ifndef SENSOR_H
#define SENSOR_H

#include <time.h>
#include "iom361_r4.h"

typedef struct reading {
    time_t t;
    float temp_c;
    float rh_pct;
} reading_t;

/*  */
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif
