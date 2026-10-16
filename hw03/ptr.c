#include "ptr.h"

void swap_float(float *a, float *b) {
    if (a == NULL || b == NULL) {
        return;
    }
    float t = *a;
    *a = *b;
    *b = t;
}

int minmax(const float *a, int n, float *min, float *max) {
    if (a == NULL || min == NULL || max == NULL || n < 1) {
        return -1;
    }

    *min = *max = *a;

    for (const float *p = a + 1; p < a + n; p++) {
        *min = *min <= *p ? *min : *p;
        *max = *max >= *p ? *max : *p;
    }

    return 0;
}
