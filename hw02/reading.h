#ifndef READING_H
#define READING_H

#include <stdio.h>
#include <string.h> // for size_t from sizeof

#define MAX_READINGS 1000 
#define LINE_LEN 128 


/*
 * Takes arrays for tick counts, temperatures, and humidity values, plus
 * pointers to the number of readings stored and the number of skipped lines.
 * Reads sensor readings from standard input, storing valid readings in the
 * arrays and updating both counters. Returns 0 after input processing.
 *
 * Assumes the arrays each have room for MAX_READINGS elements, num_readings
 * initially identifies the next available array position and is in the range
 * [0, MAX_READINGS], skipped points to a valid counter, and all pointers are
 * non-NULL.
 */
int read_lines(int ticks[], float temps[], float hums[], int *num_readings, int *skipped);

#endif
