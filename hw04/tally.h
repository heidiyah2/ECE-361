#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX 4     /* tallies that can exist at the same time */
#define TALLY_NAMES 16  /* different names one tally can hold */
#define TALLY_LEN 15    /* longest name, without the '\0' */

typedef struct tally tally_t; /* opaque */

tally_t *tally_create(void);                            /* Creates new empty tally, NULL if TALLY_MAX tallies are already in use. */
void tally_destroy(tally_t *t);                         /* Removes a tally so it can be used again, value of NULL does nothing. Assumes t came from tally_create and hasn't already been destroyed. */
int tally_add(tally_t *t, const char *name);            /* Increases count of name and returns new count; returns -1 and changes nothing if name or t is NULL, name is empty or longer than TALLY_LEN, or name is new and tally is full. Assumes t came from tally_create and hasn't already been destroyed, and name terminates with '\0'. */
int tally_count(const tally_t *t, const char *name);    /* Gives current count for name; returns 0 if never or for NULL. Assumes t came from tally_create and hasn't already been destroyed, and name terminates with '\0'. */
int tally_size(const tally_t *t);                       /* Gives number of names tally holds; 0 for NULL. Assumes t came from tally_create and hasn't already been destroyed. */
void tally_print(const tally_t *t, FILE *out);          /* One line per name, formatted "name count" in alphabetical order, does nothing if t or out is NULL. Assumes t came from tally_create and hasn't already been destroyed, and out is a stream open for writing to. */

#endif
