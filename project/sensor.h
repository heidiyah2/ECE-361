#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdint.h>
#include "iom361_r4.h"

/*-----------------------------------------------------
* Sensor module interacts with the humidity and temperature sensors.
* -----------------------------------------------------*/

/* Reads TEMP_REG and HUMID_REG of iom361, converts them (rounded to 0.1) as follows: 
* TEMP_REG: deg C = ST / 2^20 * 200 - 50, where ST is a 20-bit raw value. 
* HUMID_REG: %RH = SRH / 2^20 * 100, where SRH is a 20-bit raw value. 
* Stores them in *temp_c and *rh_pct respectively. Returns 0, or the iom361 return code (rc) if a read fails, and changes nothing.
*/
int sensor_read(uint32_t *base, float *temp_c, float *rh_pct);

#endif
