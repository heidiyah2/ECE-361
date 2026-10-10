#ifndef STATS_H
#define STATS_H

#include <stdio.h>

// Uses helper functions to find and print min, max, sum, and mean of temp array, takes temps array of float values and num_readings (count of valid lines, assumes positive count), returns nothing.
void stats_temp(const float temps[], int num_readings);

// Same as stats_temp but takes humidity array instead and print statement is tailored to humidity values.
void stats_hum(const float hums[], int num_readings);

// Finds longest run of consecutive readings above the threshold (arg of main), expects temps array, ticks array, total count of valid lines (num_readings, pos #), and the threshold (2nd arg from main, pos #).
void stats_temp_threshold(const float temps[], const int ticks[], int num_readings, float threshold);

#endif
