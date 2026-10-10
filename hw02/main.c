/*
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
 */
#include <stdlib.h> 

#include "reading.h"
#include "stats.h"
#include "histogram.h"

int main(int argc, char *argv[]) {

    int ticks[MAX_READINGS];
    float temps[MAX_READINGS];
    float hums[MAX_READINGS];

    int num_readings = 0;
    int skipped = 0;
    float threshold = 30.0f;

// ----- Checks arguments for main -----------
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

//------------------- reads lines -----------------------
    read_lines(ticks, temps, hums, &num_readings, &skipped);

    // ----- prints how many lines/readings were read -----
    printf("readings: %d\n", num_readings);
    printf("skipped:  %d\n", skipped);
    if (num_readings == 0) {
        printf("no readings, no summary\n");
        return 0;
    }

    if (num_readings == 0)
        return 0;

//------ temp, humidity, threshold -----------------------

    stats_temp(temps, num_readings);
    stats_hum(hums, num_readings);
    stats_temp_threshold(temps, ticks, num_readings, threshold);

//---------------- histogram -----------------------------
    histogram(temps, num_readings);

    return 0;
}
