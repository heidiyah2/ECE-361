#ifndef STORE_H
#define STORE_H

#include "sensor.h"
#include "fsm.h"

/*-----------------------------------------------------
* Store module handles readings, current state, and status word.
* -----------------------------------------------------*/

typedef struct store store_t;

store_t *store_create(void);
void store_destroy(store_t *s);
int store_insert(store_t *s, const state_t *current_state);
int record_compare(store_t *s);
int find_by_key(store_t *s);
int store_count(store_t *s);
int store_height(store_t *s);
int in_order_walk(store_t *s);

#endif
