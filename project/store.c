#include "store.h"
#include <stdbool.h>

#define MAX_TICKS 2000
#define MAX_STORES 4


typedef struct record {
    reading_t reading;
    state_t current_state;
    uint32_t status_word;
} record_t;

struct store {
    record_t records[MAX_TICKS];
    int record_count;
    bool in_use;
};

store_t *store_create(void) {
    /* TODO */
    return NULL;
}

void store_destroy(store_t *s) {
    (void)s; /* TODO: remove once s is used */
}

int store_insert(store_t *s, const state_t *current_state) {
    (void)s; /* TODO: remove once parameters are used */
    (void)current_state;
    return 0;
}

int record_compare(store_t *s) {
    (void)s; /* TODO */
    return 0;
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
