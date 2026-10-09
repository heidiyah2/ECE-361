#ifndef STATS_H
#define STATS_H

#include <stdio.h>

void temp_mean_min_sum(float temps[], int num_readings);
void hum_mean_min_sum(float hums[], int num_readings);
void temp_threshold(float temps[], int ticks[], int num_readings, float threshold);
#endif
