#ifndef PTR_H
#define PTR_H

#include <stdio.h>

// Exchanges *a and *b. Assumes a and b each point to a valid float, does nothing if either is NULL.
void swap_float(float *a, float *b);

// Stores the smallest and largest of a[0..n-1] in *min and *max and returns 0; if n < 1 or any pointer is NULL returns -1 and changes nothing. Assumes a points to at least n floats and min and max each point to a valid float.
int minmax(const float *a, int n, float *min, float *max);

// Returns a pointer to the first value greater than threshold, or NULL if there is none (including when a is NULL or n < 1). Assumes a points to at least n floats.
const float *find_above(const float *a, int n, float threshold);

// Reverses a[0..n-1] in place; does nothing if a is NULL or n < 2. Assumes a points to at least n floats.
void reverse(float *a, int n);

#endif
