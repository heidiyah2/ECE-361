#define _POSIX_C_SOURCE 200809L

#include "../stats.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LARGE_ARRAY_SIZE 10000
#define OUTPUT_SIZE 4096

int main(void)
{
    const float one_temp[] = {42.0f};
    const float one_humidity[] = {55.0f};
    const float end_max_temp[] = {100.0f, 1.0f, 2.0f};
    const float beginning_max_humidity[] = {100.0f, 1.0f, 2.0f};
    const float threshold_temp[] = {31.0f, 32.0f, 30.0f, 31.0f};
    const int threshold_ticks[] = {10, 11, 12, 13};
    static float large_temp[LARGE_ARRAY_SIZE];
    const char expected_one_element[] =
        "temperature: min   42.0  max   42.0  mean  42.00 C\n"
        "humidity:    min   55.0  max   55.0  mean  55.00 %RH\n";
    const char expected_max_at_ends[] =
        "temperature: min    1.0  max  100.0  mean  34.33 C\n"
        "humidity:    min    1.0  max  100.0  mean  34.33 %RH\n";
    const char expected_threshold[] =
        "above 30.0 C: longest run 2 readings, from tick 10 to tick 11\n";
    const char expected_large[] =
        "temperature: min    0.0  max 9999.0  mean 4999.29 C\n";
    char output[OUTPUT_SIZE];
    FILE *temporary_output;
    int saved_stdout;
    size_t output_length;
    int failures = 0;
    int output_ok = 1;

    for (int i = 0; i < LARGE_ARRAY_SIZE; i++)
        large_temp[i] = (float) i;

    /*
     * The stats functions print their results, so redirect stdout to a
     * temporary file while all four tests run.
     */
    fflush(stdout);
    saved_stdout = dup(fileno(stdout));
    temporary_output = tmpfile();
    if (saved_stdout == -1 || temporary_output == NULL ||
        dup2(fileno(temporary_output), fileno(stdout)) == -1) {
        output_ok = 0;
    } else {
        stats_temp(one_temp, 1);
        stats_hum(one_humidity, 1);
        stats_temp(end_max_temp, 3);
        stats_hum(beginning_max_humidity, 3);
        stats_temp_threshold(threshold_temp, threshold_ticks, 4, 30.0f);
        stats_temp(large_temp, LARGE_ARRAY_SIZE);
    }

    fflush(stdout);
    if (saved_stdout != -1 && dup2(saved_stdout, fileno(stdout)) == -1)
        output_ok = 0;
    if (saved_stdout != -1)
        close(saved_stdout);

    if (temporary_output == NULL || fseek(temporary_output, 0, SEEK_SET) != 0 ||
        (output_length = fread(output, 1, sizeof output - 1, temporary_output)) == 0) {
        output_ok = 0;
        output[0] = '\0';
    } else {
        output[output_length] = '\0';
    }
    if (temporary_output != NULL)
        fclose(temporary_output);

    if (output_ok && strstr(output, expected_one_element) != NULL)
        printf("PASS  one element in each array\n");
    else {
        printf("FAIL  one element in each array\n");
        failures++;
    }

    if (output_ok && strstr(output, expected_max_at_ends) != NULL)
        printf("PASS  maximum at beginning and end\n");
    else {
        printf("FAIL  maximum at beginning and end\n");
        failures++;
    }

    if (output_ok && strstr(output, expected_threshold) != NULL)
        printf("PASS  threshold equality ends a run\n");
    else {
        printf("FAIL  threshold equality ends a run\n");
        failures++;
    }

    if (output_ok && strstr(output, expected_large) != NULL)
        printf("PASS  recursive maximum of 10,000 values\n");
    else {
        printf("FAIL  recursive maximum of 10,000 values\n");
        failures++;
    }

    printf("Summary: 4 tests, %d failed\n", failures);
    return failures == 0 ? 0 : 1;
}
