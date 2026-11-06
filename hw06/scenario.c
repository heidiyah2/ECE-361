#include <stdio.h>
#include <string.h>
#include "scenario.h"

int scenario_load(const char *path, scenario_t *s) {
    (void)path;
    (void)s;
    return 0;
}

const scenario_line_t *scenario_at(const scenario_t *s, long tick) {
    if (s == NULL || s->count == 0 || tick < s->lines[0].tick) {
        return NULL;
    }

    const scenario_line_t *in_force = &s->lines[0]; // start with first line as default
    // replace first line as "in force" line if present
    for (int i = 1; i < s->count; i++) {
        if (s->lines[i].tick > tick)
            break;
        in_force = &s->lines[i];
    }

    return in_force;
}
