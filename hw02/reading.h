#ifndef READING_H
#define READING_H

#include <stdio.h>
#include <string.h> // for size_t from sizeof

#define MAX_READINGS 1000 
#define LINE_LEN 128 


// reads lines from data/[file], expects arrays ticks, temps, hums, address of num_readings (pos #) and address of skipped (to be able to update value for use in other functions).
int read_lines(int ticks[], float temps[], float hums[], int *num_readings, int *skipped);

#endif
