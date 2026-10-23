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


