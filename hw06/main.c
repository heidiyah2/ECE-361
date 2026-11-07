#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

#define N_SWITCHES 4 // N_SWITCHES <= N_LEDS
#define N_LEDS 4

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./replay scenario.txt\n");
        return 1;
    }
    // load file
    static scenario_t s;
    int result = scenario_load(argv[1], &s);
    if (result == -1) {
        fprintf(stderr, "can't open %s\n", argv[1]);
        return 1;
    }
    if (result > 0) {
        fprintf(stderr, "line %d is wrong in file %s\n", result, argv[1]);
        return 1;
    }
    // initialize iom361 and disable display
    int rc;
    uint32_t *base = iom361_initialize(N_SWITCHES, N_LEDS, &rc); // 4 switches, 4 LEDs
    if (base == NULL) {
        fprintf(stderr, "iom361_initialize failed: %d\n", rc);
        return 1;
    }
    iom361_set_display(false); 

    // loop from tick 0 to last tick
    long last_tick = s.lines[s.count - 1].tick;
    uint32_t before = 0;
    for (long tick = 0; tick <= last_tick; tick++) {
        const scenario_line_t *line = scenario_at(&s, tick); // find line in force

        // set world and compute now
        _iom361_set_sensor1(line->temp_c, line->rh_pct);
        _iom361_set_switches(line->switches);
        time_t now = s.start + tick;

        // read sensor and switches through registers
        reading_t reading;
        int value = sensor_read(base, now, &reading);
        if (value != 0) {
            fprintf(stderr, "error: sensor register read failed with code %d\n", value);
            return 1;
        }
        uint32_t switches = iom361_readReg(base, SWITCHES_REG, &rc);
        if (rc != 0) {
            fprintf(stderr, "error: switches register read failed with code %d\n", rc);
            return 1;
        }
        // find switches just pressed, toggle associated LED for each pressed switch
        uint32_t switches_pressed = panel_pressed(switches, before);
        for (int k = 0; k < N_SWITCHES; k++) {
            uint32_t mask = (UINT32_C(1) << k);
            if ((switches_pressed & mask) != 0) {
                int toggle_result = panel_toggle(base, k);
                if (toggle_result != 0) {
                    fprintf(stderr, "error: toggling LEDs; code %d\n", toggle_result);
                    return 1;
                }
            }
        }
        // read LEDS
        uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);
        if (rc != 0) {
            fprintf(stderr, "error: LEDs register read failed with code %d\n", rc);
            return 1;
        }
        // format time
        struct tm *utc = gmtime(&reading.t);
        char timestamp[20];
        if (utc == NULL || strftime(timestamp, sizeof timestamp, "%Y-%m-%d %H:%M:%S", utc) == 0) {
            fprintf(stderr, "error: can't format time %lld\n", (long long)reading.t);
            return 1;
        }
        // print trace line
        printf("%s temp=%.1f rh=%.1f switches=0x%" PRIX32 " pressed=0x%" PRIX32 " leds=0x%" PRIX32 "\n",
               timestamp, reading.temp_c, reading.rh_pct, switches, switches_pressed, leds);
        before = switches;
    }
    return 0;
}
