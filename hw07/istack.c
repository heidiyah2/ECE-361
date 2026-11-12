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
    (void)s;
    (void)v;
    /* TODO */
    return -1;
}

int istack_pop(istack_t *s, int *v)
{
    (void)s;
    (void)v;
    /* TODO */
    return -1;
}

int istack_peek(const istack_t *s, int *v)
{
    (void)s;
    (void)v;
    /* TODO */
    return -1;
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
