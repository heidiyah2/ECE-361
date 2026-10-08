#include <stdio.h>
#include "iom361_r4.h"

int main(void) {
    printf("Device Name: Dishwasher\n");

    int rc;
    uint32_t *base = iom361_initialize(2, 3, &rc); // 2 switches, 3 LEDs
    if (base == NULL) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        return 1;
    }

    return 0;
}