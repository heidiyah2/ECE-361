#include "stats.h"

/* largest temperature from index i to the end, recursively */
float max_temp(float temps[], int count, int i) {
    if (i == count - 1)
        return temps[i];
    float rest = max_temp(temps, count, i + 1);
    return temps[i] > rest ? temps[i] : rest;
}

/* temperature: min and mean */
void temp_mean_min_sum(float temps[], int count) {
    float tmin = temps[0];
    float tsum = 0.0f;
    for (int i = 0; i < count; i++) {
        if (temps[i] < tmin)
            tmin = temps[i];
        tsum += temps[i];
    }
    printf("temperature: min %6.1f  max %6.1f  mean %6.2f C\n", tmin, max_temp(temps, count, 0), tsum / count);

    return;
}

/* humidity: min, max, and mean, the same loop again */
void hum_mean_min_sum(float hums[], int count) {
    float hmin = hums[0];
    float hmax = hums[0];
    float hsum = 0.0f;
    for (int i = 0; i < count; i++) {
        if (hums[i] < hmin)
            hmin = hums[i];
        if (hums[i] > hmax)
            hmax = hums[i];
        hsum += hums[i];
    }
    printf("humidity:    min %6.1f  max %6.1f  mean %6.2f %%RH\n", hmin, hmax, hsum / count);
}

    /* longest run of consecutive readings above the threshold */
void temp_threshold(float temps[], int ticks[], int count, float threshold) {
    int run = 0, best = 0, best_start = -1, start = 0;
    for (int i = 0; i < count; i++) {
        if (temps[i] > threshold) {
            if (run == 0)
                start = i;
            run++;
            if (run > best) {
                best = run;
                best_start = start;
            }
        } else {
            run = 0;
        }
    }
    if (best == 0)
        printf("above %.1f C: never\n", threshold);
    else
        printf("above %.1f C: longest run %d readings, from tick %d to tick %d\n",
               threshold, best, ticks[best_start], ticks[best_start + best - 1]);
}