#ifndef ISTACK_H
#define ISTACK_H

#include <stdbool.h>
#include <stddef.h>

#define ISTACK_START 8                  /* room at first; doubles when full */
typedef struct istack istack_t;         /* opaque */

/* returns pointer to new, empty stack with room for ISTACK_START values; NULL if memory runs out (and nothing leaked). */
istack_t *istack_create(void);

/* frees the stack and everything it holds, inside out; NULL does nothing. */
void      istack_destroy(istack_t *s);

/* pushed v; if the stack is full, first doubles the room with realloc. Returns 0, or -1 if s is NULL or memory runs out: then the stack is unchanged. Assumes s points to created stack, and count and cap are accurate to contents and size of stack. */
int       istack_push(istack_t *s, int v);

/* puts tops value into *v (may be NULL) and removes it from the stack. Returns 0, or -1 if s is NULL or empty: then *v is unchanged. Assumes s is already created, and count accurately reflects data. */
int       istack_pop(istack_t *s, int *v);

/* puts tops value into *v (may be NULL). Returns 0, or -1 if s is NULL or empty: then *v is unchanged. Assumes s is already created, and count accurately reflects data. */
int       istack_peek(const istack_t *s, int *v);

/* gives current count. If s is NULL returns 0. Assumes s is already created. */
size_t    istack_count(const istack_t *s);

/* says whether stack is empty. If s is NULL returns 0. Assumes s is already created. */
bool      istack_is_empty(const istack_t *s);

/* gives capacity. If s is NULL returns 0. Assumes s is already created. */
size_t    istack_capacity(const istack_t *s);

/* returns a new array of values in s, bottom first, and their number in *n; the caller owns and frees it. Returns NULL and *n = 0 if s is NULL or empty, or memory runs out. Assumes s is created, count is accurate. */
int      *istack_to_array(const istack_t *s, size_t *n);

#endif
