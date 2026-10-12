#include <stdio.h>
#include <stdlib.h>

/*
* Run stats_max with: ./tests/depth<-flag> 0 n
* Run stats_max_tail with: ./tests/depth<-flag> 1 n
* n must be positive, run with -o0 or -o2 flags.
*
* Can run bonus.sh to find largest n for all conditions to within 12%
*/

/* Copied because this is static in stats.c
*/
static float stats_max(const float arr[], int n)
{
    if (n == 1)
        return arr[0];

    float rest = stats_max(arr + 1, n - 1);
    return arr[0] > rest ? arr[0] : rest;
}

static float stats_max_tail_helper(const float arr[], int n, float best)
{
    best = arr[0] > best ? arr[0] : best;
    if (n == 1)
        return best;

    return stats_max_tail_helper(arr + 1, n - 1, best);
}

static float stats_max_tail(const float arr[], int n)
{
    return stats_max_tail_helper(arr, n, arr[0]);
}

int main(int argc, char *argv[])
{
    int function;
    int n;
    float *values;

    if (argc != 3) {
        fprintf(stderr, "usage: %s function n\n", argv[0]);
        return 1;
    }

    function = atoi(argv[1]);
    if (function != 0 && function != 1) {
        fprintf(stderr, "error: function must be 0 (stats_max) or 1 (stats_max_tail)\n");
        return 1;
    }

    n = atoi(argv[2]);
    if (n <= 0) {
        fprintf(stderr, "error: n must be positive\n");
        return 1;
    }

    values = malloc((size_t) n * sizeof(*values));
    if (values == NULL) {
        fprintf(stderr, "error: could not allocate space for %d values\n", n);
        return 1;
    }

    for (int i = 0; i < n; i++)
        values[i] = (float) i;

    if (function == 0)
        printf("stats_max: %.1f\n", stats_max(values, n));
    else
        printf("stats_max_tail: %.1f\n", stats_max_tail(values, n));

    free(values);
    return 0;
}
