#include "sensor.h"

int main(void) {
    printf("Device Name: Dishwasher\n");

    int rc;
    uint32_t *base = iom361_initialize(2, 3, &rc); // 2 switches, 3 LEDs
    if (base == NULL) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        return 1;
    }

    _iom361_set_sensor1(54.53f, 70.76f);

    float temp_c, rh_pct;
    rc = sensor_read(base, &temp_c, &rh_pct);
    if (rc != 0) {
        fprintf(stderr, "sensor_read failed: %d\n", rc);
        return 1;
    }
    printf("Temperature: %.1f deg C\n", temp_c);
    printf("Humidity: %.1f %%RH\n", rh_pct);

    return 0;
}
