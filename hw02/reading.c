#include "reading.h"

int read_lines(int ticks[], float temps[], float hums[], int *count, int *skipped) {
    char line[LINE_LEN];
// ------ reads lines -------
    while (fgets(line, sizeof line, stdin) != NULL) {
        int i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;
        if (*count == MAX_READINGS) {
            fprintf(stderr, "warning: more than %d readings, the rest are ignored\n", MAX_READINGS);
            break;
        }
        if (sscanf(line, "%d %f %f", &ticks[*count], &temps[*count], &hums[*count]) == 3)
            (*count)++;
        else
            (*skipped)++;
    }

    return 0;
}
