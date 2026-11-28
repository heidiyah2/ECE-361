#ifndef CLOCK_H
#define CLOCK_H

#include <stddef.h>
#include <time.h>

/*-----------------------------------------------------
* CLOCK module keeps time as a time_t and formats timestamps for the trace.
* -----------------------------------------------------*/

/* "YYYY-MM-DD HH:MM:SS" is 19 characters, plus '\0'. */
#define CLOCK_STR_SIZE 20

typedef struct sim_clock {
    time_t now;
} sim_clock_t;

/* Sets the clock to start. Does nothing if c is NULL. */
void clock_init(sim_clock_t *c, time_t start);

/* Advances the clock by one second. Does nothing if c is NULL. */
void clock_tick(sim_clock_t *c);

/* Current time, or (time_t)-1 if c is NULL. */
time_t clock_now(const sim_clock_t *c);

/* Writes t as "YYYY-MM-DD HH:MM:SS" (UTC) into buf. Returns 0, or -1 if buf is NULL, gmtime fails, or size is too small. */
int clock_format(time_t t, char *buf, size_t size);

#endif
