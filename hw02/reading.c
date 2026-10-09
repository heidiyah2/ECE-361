#include "reading.h"

int read_lines(int ticks[], float temps[], float hums[], int *num_readings, int *skipped) {
    char line[LINE_LEN];
// ------ reads lines -------
    while (fgets(line, sizeof line, stdin) != NULL) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;
        if (*num_readings == MAX_READINGS) {
            fprintf(stderr, "warning: more than %d readings, the rest are ignored\n", MAX_READINGS);
            break;
        }
        if (sscanf(line, "%d %f %f", &ticks[*num_readings], &temps[*num_readings], &hums[*num_readings]) == 3)
            (*num_readings)++;
        else
            (*skipped)++;
    }

    return 0;
}
