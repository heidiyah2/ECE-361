#include "clock.h"

void clock_init(sim_clock_t *c, time_t start) {
    if (c == NULL)
        return;
    c->now = start;     // stores start time from scenario
}

void clock_tick(sim_clock_t *c) {
    if (c == NULL)
        return;
    c->now++;
}

time_t clock_now(const sim_clock_t *c) {
    if (c == NULL)
        return (time_t)-1;      // Dec 1969 (clearly an error)
    return c->now;
}

int clock_format(time_t t, char *buf, size_t size) {
    if (buf == NULL)
        return -1;

    struct tm *tm = gmtime(&t);     // break into date & time, returns NULL if year can't fit in int field of struct tm (hence check next)
    if (tm == NULL || strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm) == 0) {  //size 0 also gives 0
        // buf may hold garbage after a failed strftime (returns 0), leave it an empty string so print is safe just in case
        if (size > 0)
            buf[0] = '\0';
        return -1;
    }
    return 0;
}
