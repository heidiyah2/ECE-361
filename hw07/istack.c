#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "istack.h"

struct istack {
    int    *data;   /* the values, on the heap; data[0] is the bottom */
    size_t  count;  /* values held right now */
    size_t  cap;    /* room in data, in ints */
};

istack_t *istack_create(void)
{
    istack_t *s = malloc(sizeof *s);
    // ALWAYS check
    if (s == NULL) {
        return NULL;
    }
    // allocate data[]
    s->data = malloc(ISTACK_START * sizeof *s->data);
    // check!
    if (s->data == NULL) {
        free(s);    // have to free s or we have memory leak
        return NULL;
    }
    s->count = 0;
    s->cap = ISTACK_START;
    return s;
}

void istack_destroy(istack_t *s)
{
    if (s == NULL) return;
    free(s->data);      // the inside first
    free(s);            // then the struct
}

int istack_push(istack_t *s, int v)
{
    if (s == NULL) {
        return -1;
    }
    // check if stack is full, double if needed
    if (s->count == s->cap) {
        size_t double_cap = 2 * s->cap;
        int *d = realloc(s->data, double_cap * sizeof *s->data);
        if (d == NULL) {
            return -1;
        }
        // safe to update now
        s->data = d;
        s->cap = double_cap;
    }
    s->data[s->count] = v;
    ++s->count;
    return 0;
}

int istack_pop(istack_t *s, int *v)
{
    if (istack_peek(s, v) != 0) 
        return -1;
    --s->count;
    return 0;
}

int istack_peek(const istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) {   // count access after NULL check, safe
        return -1;
    }
    size_t top = s->count - 1;  // data contains 0..count-1
    if (v != NULL) {
        *v = s->data[top];
    }
    return 0;
}

size_t istack_count(const istack_t *s)
{
    (void)s;
    /* TODO */
    return 0;
}

bool istack_is_empty(const istack_t *s)
{
    (void)s;
    /* TODO */
    return true;
}

size_t istack_capacity(const istack_t *s)
{
    (void)s;
    /* TODO */
    return 0;
}

int *istack_to_array(const istack_t *s, size_t *n)
{
    (void)s;
    /* TODO */
    if (n != NULL) {
        *n = 0;
    }
    return NULL;
}
