#include "store.h"
#include <stdbool.h>

#define MAX_TICKS 2000

struct store {
    record_t records[MAX_TICKS];
    int record_count;
    bool in_use;
} store_t;