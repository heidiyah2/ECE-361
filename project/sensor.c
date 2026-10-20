#include <math.h>
#include "sensor.h"

#define TEMP_REG 0x0C
#define HUMID_REG 0x10

int sensor_read(uint32_t *base, float *temp_c, float *rh_pct) {
    int rc;
    
    // read temp & humidity
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);
    if (rc != 0)
        return rc;

    uint32_t raw_hum = iom361_readReg(base, HUMID_REG, &rc);
    if (rc != 0)
        return rc;

    *temp_c = raw_temp / 1048576.0f * 200 - 50; // .0f so fraction is retained (c floors divisions)
    *rh_pct = raw_hum / 1048576.0f * 100;

    // round to 0.1: scale up, round to nearest integer, scale back down
    *temp_c = roundf(*temp_c * 10.0f) / 10.0f;
    *rh_pct = roundf(*rh_pct * 10.0f) / 10.0f;

    return 0;
}
