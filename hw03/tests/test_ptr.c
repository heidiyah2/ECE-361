#include "../ptr.h"

#include <stdio.h>

/* Values that minmax must leave alone when it returns -1. */
#define SENTINEL_MIN 111.0f
#define SENTINEL_MAX 222.0f

static int tests_run = 0;
static int failures = 0;

/* Prints PASS or FAIL for one test and counts the result. */
static void check(int passed, const char *name)
{
    tests_run++;
    if (passed)
        printf("PASS  %s\n", name);
    else {
        printf("FAIL  %s\n", name);
        failures++;
    }
}

/* Returns 1 if a[0..n-1] and b[0..n-1] hold the same values, 0 otherwise. */
static int same_values(const float *a, const float *b, int n)
{
    for (const float *p = a, *q = b; p < a + n; p++, q++) {
        if (*p != *q)
            return 0;
    }
    return 1;
}

static void test_swap_float(void)
{
    float x = 1.5f;
    float y = -2.25f;
    float z = 7.0f;

    swap_float(&x, &y);
    check(x == -2.25f && y == 1.5f, "swap_float exchanges two values");

    swap_float(&z, &z);
    check(z == 7.0f, "swap_float with the same address twice");

    x = 1.5f;
    swap_float(NULL, &x);
    check(x == 1.5f, "swap_float with NULL a leaves b unchanged");

    swap_float(&x, NULL);
    check(x == 1.5f, "swap_float with NULL b leaves a unchanged");
}

static void test_minmax(void)
{
    const float several[] = {3.0f, -1.0f, 7.0f, 2.0f};
    const float max_first_min_last[] = {9.0f, 4.0f, -6.0f};
    const float all_negative[] = {-5.0f, -2.0f, -9.0f};
    const float all_equal[] = {4.0f, 4.0f, 4.0f};
    const float one[] = {42.0f};
    float min;
    float max;
    int result;

    result = minmax(several, 4, &min, &max);
    check(result == 0 && min == -1.0f && max == 7.0f,
          "minmax on several values");

    result = minmax(max_first_min_last, 3, &min, &max);
    check(result == 0 && min == -6.0f && max == 9.0f,
          "minmax with maximum first and minimum last");

    result = minmax(all_negative, 3, &min, &max);
    check(result == 0 && min == -9.0f && max == -2.0f,
          "minmax on all negative values");

    result = minmax(all_equal, 3, &min, &max);
    check(result == 0 && min == 4.0f && max == 4.0f,
          "minmax on all equal values");

    result = minmax(one, 1, &min, &max);
    check(result == 0 && min == 42.0f && max == 42.0f,
          "minmax on one value");

    min = SENTINEL_MIN;
    max = SENTINEL_MAX;
    result = minmax(several, 0, &min, &max);
    check(result == -1 && min == SENTINEL_MIN && max == SENTINEL_MAX,
          "minmax with n = 0 returns -1, outputs unchanged");

    result = minmax(several, -3, &min, &max);
    check(result == -1 && min == SENTINEL_MIN && max == SENTINEL_MAX,
          "minmax with negative n returns -1, outputs unchanged");

    result = minmax(NULL, 4, &min, &max);
    check(result == -1 && min == SENTINEL_MIN && max == SENTINEL_MAX,
          "minmax with NULL a returns -1, outputs unchanged");

    result = minmax(several, 4, NULL, &max);
    check(result == -1 && max == SENTINEL_MAX,
          "minmax with NULL min returns -1, max unchanged");

    result = minmax(several, 4, &min, NULL);
    check(result == -1 && min == SENTINEL_MIN,
          "minmax with NULL max returns -1, min unchanged");
}

static void test_find_above(void)
{
    const float values[] = {1.0f, 5.0f, 2.0f, 8.0f};
    const float negative[] = {-5.0f, -1.0f, 3.0f};

    check(find_above(values, 4, 4.0f) == values + 1,
          "find_above with two matches returns the first");

    check(find_above(values, 4, 10.0f) == NULL,
          "find_above with no match returns NULL");

    check(find_above(values, 4, 8.0f) == NULL,
          "find_above with a value equal to the threshold returns NULL");

    check(find_above(values, 4, 0.0f) == values,
          "find_above with a match at the first element");

    check(find_above(values, 4, 7.0f) == values + 3,
          "find_above with a match at the last element");

    check(find_above(negative, 3, -2.0f) == negative + 1,
          "find_above with a negative threshold");

    check(find_above(values, 0, 0.0f) == NULL,
          "find_above with n = 0 returns NULL");

    check(find_above(NULL, 4, 0.0f) == NULL,
          "find_above with NULL a returns NULL");
}

static void test_reverse(void)
{
    float even[] = {1.0f, 2.0f, 3.0f, 4.0f};
    const float even_reversed[] = {4.0f, 3.0f, 2.0f, 1.0f};
    float odd[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    const float odd_reversed[] = {5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
    float descending[] = {4.0f, 3.0f, 2.0f, 1.0f};
    const float descending_reversed[] = {1.0f, 2.0f, 3.0f, 4.0f};
    float two[] = {1.0f, 2.0f};
    const float two_reversed[] = {2.0f, 1.0f};
    float one[] = {7.0f};
    float untouched[] = {1.0f, 2.0f, 3.0f};
    const float untouched_original[] = {1.0f, 2.0f, 3.0f};

    reverse(even, 4);
    check(same_values(even, even_reversed, 4), "reverse on an even length");

    reverse(odd, 5);
    check(same_values(odd, odd_reversed, 5), "reverse on an odd length");

    reverse(descending, 4);
    check(same_values(descending, descending_reversed, 4),
          "reverse on values already in descending order");

    reverse(two, 2);
    check(same_values(two, two_reversed, 2), "reverse on two values");

    reverse(one, 1);
    check(one[0] == 7.0f, "reverse on one value does nothing");

    reverse(untouched, 0);
    check(same_values(untouched, untouched_original, 3),
          "reverse with n = 0 does nothing");

    reverse(NULL, 4);
    check(1, "reverse with NULL a does not crash");
}

int main(void)
{
    test_swap_float();
    test_minmax();
    test_find_above();
    test_reverse();

    printf("Summary: %d tests, %d failed\n", tests_run, failures);
    return failures == 0 ? 0 : 1;
}
