#include "panel.h"
#include "iom361_r4.h"

uint32_t panel_pressed(uint32_t now, uint32_t before) {
    return now & ~before;
}

uint32_t panel_released(uint32_t now, uint32_t before) {
    return before & ~now;
}

int panel_led(uint32_t *base, int n, int on) {
    if (n > 31 || n < 0) {
        return -1;
    }
    int rc;
    // read
    uint32_t led_reg = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0) 
        return rc;
    
    // modify
    uint32_t mask = UINT32_C(1) << n;
    if (on != 0) 
        led_reg |= mask;
    else
        led_reg &= ~mask;

    // write
    iom361_writeReg(base, LEDS_REG, led_reg, &rc);
    if (rc != 0) 
        return rc;

    return 0;
}

int panel_toggle(uint32_t *base, int n) {
    if (n > 31 || n < 0) {
        return -1;
    }
    int rc;
    // read
    uint32_t led_reg = iom361_readReg(base, LEDS_REG, &rc);
    if (rc != 0) 
        return rc;

    //modify
    uint32_t mask = UINT32_C(1) << n;
    led_reg ^= mask;

    //write 
    iom361_writeReg(base, LEDS_REG, led_reg, &rc);
    if (rc != 0) 
        return rc;

    return 0;
}
