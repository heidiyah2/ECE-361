/*
 * readings.c: ECE 361, Fall 2026, HW2 starter
 *
 * Reads sensor readings from standard input and prints a summary: the number
 * of readings, the minimum, maximum, and mean temperature and humidity, the
 * longest run of readings above a temperature threshold, and a histogram of
 * the temperatures.
 *
 * Input: one reading per line, "tick temperature humidity", for example
 *     12  23.5  41.0
 * Blank lines and lines starting with '#' are ignored. A line that does not
 * hold three numbers is counted as skipped.
 *
 * Usage: ./readings [threshold] < data/normal.txt
 *        threshold is in degrees C, default 30.0
 *
 * The program works. It is also one file, with global variables, repeated
 * code, and a main() that does everything. HW2 asks you to split it into
 * modules without changing what it prints.
 */
#include <stdlib.h> // 
//#include <string.h> // not used

#include "reading.h"
#include "stats.h"
#include "histogram.h"

#define LINE_LEN     128 // only for command arg check in main

int   skipped = 0; // reading.c


int main(int argc, char *argv[]) {

    int   ticks[MAX_READINGS]; // used to read lines, and print run of cons readings
    float temps[MAX_READINGS]; // read lines, stats, and histogram
    float hums[MAX_READINGS]; // read lines, temp

    int   count = 0; // make local only
    float threshold = 30.0f; // command line, pass to stats for cons lines

// ----- Checks arguments for main -----------
    char line[LINE_LEN];

    if (argc > 2) {
        fprintf(stderr, "usage: %s [threshold] < readings.txt\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        char *end;
        threshold = strtof(argv[1], &end);
        if (*end != '\0') {
            fprintf(stderr, "error: threshold '%s' is not a number\n", argv[1]);
            return 1;
        }
    }
// ----------------------------------------------------------

// ------ reads lines -------
    while (fgets(line, sizeof line, stdin) != NULL) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;
        if (count == MAX_READINGS) {
            fprintf(stderr, "warning: more than %d readings, the rest are ignored\n", MAX_READINGS);
            break;
        }
        if (sscanf(line, "%d %f %f", &ticks[count], &temps[count], &hums[count]) == 3)
            count++;
        else
            skipped++;
    }
// ----- prints how many lines/readings were read -----
    printf("readings: %d\n", count);
    printf("skipped:  %d\n", skipped);
    if (count == 0) {
        printf("no readings, no summary\n");
        return 0;
    }

//------ temp, humidity, threshold -----------------------

    temp_mean_min_sum(temps, count);
    hum_mean_min_sum(hums, count);
    temp_threshold(temps, ticks, count, threshold);

//--------------------------------------------------------------
    histogram(temps, count);

    return 0;
}
