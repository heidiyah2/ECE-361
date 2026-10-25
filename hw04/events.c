#include "tally.h"
#include <string.h>

#define LINE_LEN 128

int main(void) {
    char line[LINE_LEN];
    int skipped = 0;

    tally_t *t = tally_create();
    if (t == NULL) {
        fprintf(stderr, "Out of space for new tally\n");
        return 1;
    }

    while (fgets(line, sizeof line, stdin) != NULL) {
        size_t line_len = strlen(line);
        if (line_len > 0) {
            if (line[line_len - 1] == '\n') {
                line[line_len - 1] = '\0';
            }
        }
        if (strcmp(line,"") == 0) {
            continue;
        }
        if (line[0] == '#') {
            continue;
        }
        int add = tally_add(t, line);
        if (add == -1) {
            skipped++;
        }
    }
    tally_print(t, stdout);
    fprintf(stdout, "names: %d, lines skipped: %d\n", tally_size(t), skipped);

    return 0;
}
