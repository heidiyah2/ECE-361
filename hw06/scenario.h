#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <time.h>

#define SCENARIO_MAX_LINES 1000

typedef struct scenario_line {
    long tick;
    float temp_c;
    float rh_pct;
    uint32_t switches;
} scenario_line_t;

typedef struct scenario {
    time_t start;
    scenario_line_t lines[SCENARIO_MAX_LINES];
    int count;
} scenario_t;

int scenario_load(const char *path, scenario_t *s);
const scenario_line_t *scenario_at(const scenario_t *s, long tick);

#endif
