#ifndef STATS_H
#define STATS_H

#include <stdio.h>

/*
 * Takes an array of temperatures and the number of readings to process.
 * Finds and prints the minimum, maximum, and mean temperature. Returns
 * nothing.
 *
 * Assumes temps contains at least num_readings elements, num_readings is
 * greater than zero, and temps is non-NULL.
 */
void stats_temp(const float temps[], int num_readings);

/*
 * Takes an array of humidity values and the number of readings to process.
 * Finds and prints the minimum, maximum, and mean humidity. Returns nothing.
 *
 * Assumes hums contains at least num_readings elements, num_readings is
 * greater than zero, and hums is non-NULL.
 */
void stats_hum(const float hums[], int num_readings);

/*
 * Takes parallel arrays of temperatures and tick counts, the number of
 * readings to process, and a temperature threshold. Finds and prints the
 * longest consecutive run of temperatures above the threshold, including the
 * tick at the beginning and end of the run when one exists. Returns nothing.
 *
 * Assumes temps and ticks each contain at least num_readings elements,
 * num_readings is non-negative, and both arrays are non-NULL when
 * num_readings is greater than zero.
 */
void stats_temp_threshold(const float temps[], const int ticks[], int num_readings, float threshold);

/*--------------------- for bonus problem-------------------*/
// alternate version of static stats_max that makes recursion a tail call, preventing each call from stacking frames on the stack, instead each call behaves in memory as a normal function call would. Expects the same inputs as stats_temp/hum, returns float best_value (pos #).
float stats_max_tail(const float a[], int n);

#endif
