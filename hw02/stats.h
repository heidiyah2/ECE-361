#ifndef STATS_H
#define STATS_H

#include <stdio.h>

float max_temp(float temps[], int count, int i);
void temp_mean_min_sum(float temps[], int count);
void hum_mean_min_sum(float hums[], int count);
void temp_threshold(float temps[], int ticks[], int count, float threshold);
#endif
