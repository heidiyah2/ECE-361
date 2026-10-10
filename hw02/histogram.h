#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>

#define NUM_BINS 20
#define BIN_WIDTH 5.0f // 5 deg width

// prints histogram of temperatures in 5 C bins; values outside 0..100 go to the end bins, expects temps array and num_readings (pos count of valid lines).
void histogram(const float temps[], int num_readings);

#endif
