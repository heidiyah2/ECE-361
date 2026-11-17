#ifndef STORE_H
#define STORE_H

#include <stdint.h>
#include "sensor.h"
#include "fsm.h"

/*-----------------------------------------------------
* Store module handles readings, current state, and status word.
* -----------------------------------------------------*/

/* One record per tick. Callers build records and read them. */
typedef struct record {
    reading_t reading;
    state_t current_state;
    uint32_t status_word;
} record_t;

/* Opaque: only store.c knows what is inside. */
typedef struct store store_t;

/* creates new, empty store and returns pointer to it. Returns NULL if MAX_STORES reached. */
store_t *store_create(void);
void store_destroy(store_t *s);
/* Inserts a copy of *r in key order (state, then timestamp). Records with equal
 * keys are kept in order of arrival. Returns 0 on success, -1 if s or r is NULL,
 * s is not an active store, or the store is full. */
int store_insert(store_t *s, const record_t *r);

/* Compares two records by state, then by timestamp.
 * Returns <0 if a comes before b, 0 if equal keys, >0 if a comes after b.
 * Precondition: a and b must not be NULL. */
int record_compare(const record_t *a, const record_t *b);
int find_by_key(store_t *s);
int store_count(store_t *s);
int store_height(store_t *s);
int in_order_walk(store_t *s);

#endif
