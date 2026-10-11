#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>

#define NUM_BINS 20
#define BIN_WIDTH 5.0f // 5 deg width

/*
 * Takes an array of temperatures and the number of readings to process.
 * Prints a histogram using 5 C bins; temperatures below 0 C go in the first
 * bin and temperatures at or above 100 C go in the last bin. Returns nothing.
 *
 * Assumes temps contains at least num_readings elements, num_readings is
 * non-negative, and temps is non-NULL when num_readings is greater than zero.
 */
void histogram(const float temps[], int num_readings);

#endif
