#include <math.h>
#include "sensor.h"

#define TEMP_REG 0x0C
#define HUMID_REG 0x10

int sensor_read(uint32_t *base, time_t now, reading_t *r) {
    int rc, bits_wanted = 20;
    
    // read temp & humidity
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0)
        return rc;

    uint32_t raw_hum = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0)
        return rc;

    // mask lower 20 bits
    raw_temp &= (UINT32_C(1) << bits_wanted) - 1;
    raw_hum &= (UINT32_C(1) << bits_wanted) - 1;

    r->temp_c = raw_temp / 1048576.0f * 200 - 50; // .0f so fraction is retained (c truncates int divisions toward 0)
    r->rh_pct = raw_hum / 1048576.0f * 100;

    // round to 0.1: scale up, round to nearest integer, scale back down
    r->temp_c = roundf(r->temp_c * 10.0f) / 10.0f;
    r->rh_pct = roundf(r->rh_pct * 10.0f) / 10.0f;
    r->t = now;

    return 0;
}
