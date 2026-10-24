#include "tally.h"
#include <string.h>
#include <stdbool.h>

typedef struct entry {
    char name[TALLY_LEN + 1];
    int count;
} entry_t;

struct tally {
    entry_t entries[TALLY_NAMES];
    int num_names;
    bool in_use;
};

static tally_t pool[TALLY_MAX];

tally_t *tally_create(void) {
    for (int i = 0; i < TALLY_MAX; i++) {
        if (!pool[i].in_use) {
            pool[i].in_use = true;
            pool[i].num_names = 0;
            return &pool[i];
        }
    }
    return NULL;
}

void tally_destroy(tally_t *t) {
    if (t == NULL) {
        return;
    }
    t->in_use = false; // t points to pool[i] and selects in_use
}

int tally_count(const tally_t *t, const char *name) {
    if (t == NULL || name == NULL) {
        return 0;
    }
    for (int i = 0; i < t->num_names; i++) {
        if (strcmp(name, t->entries[i].name) == 0) {
            return t->entries[i].count;
        }
    }
    return 0;
}

int tally_size(const tally_t *t) {
    if (t == NULL) {
        return 0;
    }
    return t->num_names;
}

int tally_add(tally_t *t, const char *name) {
    // basic checks
    if (t == NULL || name == NULL || strlen(name) == 0 || strlen(name) > TALLY_LEN) {
        return -1;
    }
    // find match or insert spot
    int insert = t->num_names;
    for (int i = 0; i < t->num_names; i++) {
        if (strcmp(name, t->entries[i].name) == 0) {
            return ++t->entries[i].count;
        }
        if (strcmp(name, t->entries[i].name) < 0) {
            insert = i;
            break;
        }
    }
    // check if tally is full
    if (t->num_names == TALLY_NAMES) {
        return -1;
    }
    // shift names and insert new name
    for (int j = t->num_names; j > insert; j--) {
        t->entries[j] = t->entries[j - 1];
    }
    strcpy(t->entries[insert].name, name);
    t->entries[insert].count = 1;
    t->num_names++;
    return 1;
}
