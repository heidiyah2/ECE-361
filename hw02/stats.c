#include "stats.h"

// struct to contain temp and humidity stats
struct stats_summary {
    float min;
    float max;
    float sum;
};

// finds max of given temp or humidity, recursively
static float stats_max(const float arr[], int num_readings) {
    if (num_readings == 1)
        return arr[0];
    float rest = stats_max(arr + 1, num_readings - 1); // advance arr pointer

    return arr[0] > rest ? arr[0] : rest;
}

// finds min and sum of desired array, calls stats_max & assigns stats to struct stats_summary
static struct stats_summary stats_min_sum_mean(const float arr[], int num_readings) {
    struct stats_summary result;
    result.min = arr[0];
    result.sum = 0.0f;

    for (int i = 0; i < num_readings; i++) {
        if (arr[i] < result.min)
            result.min = arr[i];
        result.sum += arr[i];
    }
    result.max = stats_max(arr, num_readings);
    return result;
}

/* temperature: min and mean */
void stats_temp(const float temps[], int num_readings) {
    struct stats_summary result = stats_min_sum_mean(temps, num_readings);

    printf("temperature: min %6.1f  max %6.1f  mean %6.2f C\n",
           result.min, result.max, result.sum / num_readings);
}

/* humidity: min, max, and mean */
void stats_hum(const float hums[], int num_readings) {
    struct stats_summary result = stats_min_sum_mean(hums, num_readings);

    printf("humidity:    min %6.1f  max %6.1f  mean %6.2f %%RH\n",
           result.min, result.max, result.sum / num_readings);
}

/* longest run of consecutive readings above the threshold */
void stats_temp_threshold(const float temps[], const int ticks[], int num_readings, float threshold) {
    int run = 0, best = 0, best_start = -1, start = 0;
    for (int i = 0; i < num_readings; i++) {
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

/*---------------------- bonus problem ------------------------*/
// recursive helper that carries best_value w/ current maximum
static float stats_max_tail_helper(const float a[], int n, float best_value) {
    
    best_value = a[0] > best_value ? a[0] : best_value;
    if (n == 1)
        return best_value;

    return stats_max_tail_helper(a + 1, n - 1, best_value);
}

// recursive maximum w/ helper that carries best value (tail call optimization)
float stats_max_tail(const float a[], int n) {
    float best_value = a[0];

    best_value = stats_max_tail_helper(a, n, best_value);

    return best_value;
}