#include "stats.h"

/* largest temperature from index i to the end, recursively */
float max_temp(float temps[], int count, int i) {
    if (i == count - 1)
        return temps[i];
    float rest = max_temp(temps, count, i + 1);
    return temps[i] > rest ? temps[i] : rest;
}
