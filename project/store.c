#include "store.h"
#include <stdbool.h>

#define MAX_TICKS 2000
#define MAX_STORES 4

struct store {
    record_t records[MAX_TICKS];
    int record_count;
    bool in_use;
};

/* Fixed pool of stores: no malloc. static keeps it private to this file. */
static store_t store_pool[MAX_STORES];

store_t *store_create(void) {
    for (int i = 0; i < MAX_STORES; i++) {
        if (!store_pool[i].in_use) {
            store_pool[i].record_count = 0;
            store_pool[i].in_use = true;
            return &store_pool[i];
        }
    }
    return NULL; /* all MAX_STORES stores are in use */
}

void store_destroy(store_t *s) {
    if (s == NULL) {
        return;
    }
    s->in_use = false;
}

int store_insert(store_t *s, const record_t *r) {
    if (s == NULL || r == NULL || !s->in_use) {
        return -1;
    }
    if (s->record_count >= MAX_TICKS) {
        return -1; /* store is full */
    }

    /* Start at the end; shift every record that belongs after r one slot right.
     * Strictly greater (> 0), so an equal key stays in front: arrival order kept. */
    int i = s->record_count;
    while (i > 0 && record_compare(&s->records[i - 1], r) > 0) {
        s->records[i] = s->records[i - 1];
        i--;
    }

    s->records[i] = *r; /* copy the record into the gap */
    s->record_count++;
    return 0;
}

int record_compare(const record_t *a, const record_t *b) {
    /* Key 1: state */
    if (a->current_state < b->current_state) {
        return -1;
    }
    if (a->current_state > b->current_state) {
        return 1;
    }

    /* Key 2: timestamp (only reached when states are equal). */
    if (a->reading.timestamp < b->reading.timestamp) {
        return -1;
    }
    if (a->reading.timestamp > b->reading.timestamp) {
        return 1;
    }

    return 0; /* equal keys */
}

int find_by_key(store_t *s) {
    (void)s; /* TODO */
    return 0;
}

int store_count(store_t *s) {
    (void)s; /* TODO */
    return 0;
}

int store_height(store_t *s) {
    (void)s; /* TODO */
    return 0;
}

int in_order_walk(store_t *s) {
    (void)s; /* TODO */
    return 0;
}
