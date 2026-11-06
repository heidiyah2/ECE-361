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

/* Loads the scenario file at path into *s: # comments and blank lines are skipped, the first other line must be "start <timestamp>", then "tick temp rh switches" lines with switches in hex, the first tick 0, and ticks strictly increasing, at most SCENARIO_MAX_LINES. Returns 0, -1 if path or s is NULL or the file can't be opened, or the number of the first wrong line (the line after the last if start or data is missing); after a failure *s must not be used. A line longer than 255 characters is a wrong line. */
int scenario_load(const char *path, scenario_t *s);
/* Returns the line in force at tick: the last line whose tick is <= tick, so values hold between lines. Returns NULL if s is NULL, has no lines, or tick is before the first line. Assumes *s was loaded by scenario_load (ticks increasing); the pointer is valid while *s is unchanged. */
const scenario_line_t *scenario_at(const scenario_t *s, long tick);

#endif
