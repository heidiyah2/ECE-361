#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include "scenario.h"

int scenario_load(const char *path, scenario_t *s) {
    if (path == NULL || s == NULL) {
        return -1;
    }
    // open file
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    // read lines
    s->count = 0;
    int counter = 0;
    bool seen_start = false;
    char buf[256];
    int result = 0;

    while (fgets(buf, sizeof buf, file) != NULL) {
        counter++; // first so blank lines and comments are counted
        // check if end of file
        if (strchr(buf, '\n') == NULL && !feof(file)) {
            result = counter;
            break;
        }
        // full-line comments turned to empty line, EOL comments cut off
        char *hash = strchr(buf, '#');
        if (hash != NULL)
            *hash = '\0';
        // skip blank lines
        if (strspn(buf, " \t\r\n") == strlen(buf))
            continue;

        // 
        int end = 0;

        if (!seen_start) {
            long long start;
            if (sscanf(buf, " start %lld %n", &start, &end) != 1 || buf[end] != '\0') { // accepted format: start 1745825
                result = counter;
                break;
            }
            s->start = (time_t)start; // cast required, no scanf format for time_t
            seen_start = true;
            continue;
        }
        // parse data line; SCNx32 macro for reading hex value on any system
        scenario_line_t line;
        if (sscanf(buf, "%ld %f %f %" SCNx32 " %n", &line.tick, &line.temp_c, &line.rh_pct, &line.switches, &end) != 4 || buf[end] != '\0') {
            result = counter;
            break;
        }

        if (s->count == 0 && line.tick != 0) {      // first line has to be tick 0
            result = counter;
            break;
        }
        if (s->count > 0 && line.tick <= s->lines[s->count - 1].tick) {     // ticks must be increasing
            result = counter;
            break;
        }
        if (s->count == SCENARIO_MAX_LINES) {       // check if array is full
            result = counter;
            break;
        }
        // store line
        s->lines[s->count] = line;
        s->count++;
    }
    // if no line failed and: the file has nothing in it or there was start with no data
    if (result == 0 && (!seen_start || s->count == 0))
        result = counter + 1;

    fclose(file);
    return result;
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
