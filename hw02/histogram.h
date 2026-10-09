#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>

#define NUM_BINS     20          /* histogram: 0 to 100 C in 5 C bins */ // histogram.h
#define BIN_WIDTH    5.0f // histogram.h

void histogram(float temps[], int num_readings);

#endif
