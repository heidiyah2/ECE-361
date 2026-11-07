#ifndef PANEL_H
#define PANEL_H

#include <stdint.h>

/* determines which switches went from off to on. Now is the reading from current tick and before from previous tick. Returns a bitmask with bit k set for each switch k just pressed, 0 if none. Assumes bit k is switch k, and 1 is on. */
uint32_t panel_pressed(uint32_t now, uint32_t before);
/* determines which switches went from on to off. Now is the reading from current tick and before from previous tick. Returns a bitmask with bit k set for each switch k just released, 0 if none. Assumes bit k is switch k, and 1 is on. */
uint32_t panel_released(uint32_t now, uint32_t before);
/* turns LED n on or off by read-modify-write, so all other LEDs keep their state; returns 0 on success, -1 if n is not 0..31, or rc code for failed read/write. If on is not zero turns LED n on. Assumes iom361 has been initialized, LED n is bit n, 1 is lit. */
int panel_led(uint32_t *base, int n, int on);
/* toggles LED n by read-modify-write the same way as panel_led; returns the same values as panel_led. */
int panel_toggle(uint32_t *base, int n);

#endif
