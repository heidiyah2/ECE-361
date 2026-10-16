#include "ptr.h"

void swap_float(float *a, float *b) {
    if (a == NULL || b == NULL) {
        return;
    }
    float t = *a;
    *a = *b;
    *b = t;
}
