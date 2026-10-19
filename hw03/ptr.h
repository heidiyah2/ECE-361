#ifndef PTR_H
#define PTR_H

#include <stdio.h>

// Exchanges *a and *b. Expects non-NULL a and b.
void swap_float(float *a, float *b);

// Stores the smallest and largest of a[0..n-1] in *min and *max and returns 0; if n < 1 or any pointer is NULL returns -1. Expects non-NULL a, min, and max, and n >= 1.
int minmax(const float *a, int n, float *min, float *max);

// Returns a pointer to the first value greater than threshold, or NULL if there is none. Expects non-NULL a, and n >= 1.
const float *find_above(const float *a, int n, float threshold);

// Reverses a[0..n-1] in place; does nothing if a is NULL or n < 2. Expects non-NULL a, n >= 2.
void reverse(float *a, int n);

#endif
