#ifndef READING_H
#define READING_H

#include <stdio.h>
#include <string.h> // for size_t from sizeof

#define MAX_READINGS 1000 // only in read lines, reading.h
#define LINE_LEN     128 

int read_lines(int ticks[], float temps[], float hums[], int *num_readings, int *skipped);

#endif
