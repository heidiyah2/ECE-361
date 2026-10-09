#include "status.h"
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned int passed;
    unsigned int failed;
} test_summary_t;

static unsigned int total_passed;
static unsigned int total_failed;

static void record_result(test_summary_t *summary, const char *name, int passed)
{
    if (passed) {
        printf("Testing %s: PASS\n", name);
        ++summary->passed;
        ++total_passed;
    } else {
        printf("Testing %s: FAIL\n", name);
        ++summary->failed;
        ++total_failed;
    }
}

static void print_summary(const char *category, test_summary_t summary)
{
    printf("%s summary: %u passed, %u failed\n\n",
        category, summary.passed, summary.failed);
}

static void check_uint32(test_summary_t *summary, const char *name,
    uint32_t actual, uint32_t expected)
{
    record_result(summary, name, actual == expected);
}

static void check_int32(test_summary_t *summary, const char *name,
    int32_t actual, int32_t expected)
{
    record_result(summary, name, actual == expected);
}

static void test_print_binary(void)
{
    printf("=== print_binary (visual checks) ===\n");
    // Check formatted output and rejection of widths outside 1 through 32.
    printf("Testing width 8; expected: 0010 1100; actual:\n");
    print_binary(0x2C, 8);
    printf("Testing width below range; expected: Cannot print binary; width is not between 1 and 32; actual:\n");
    print_binary(0x2C, -1);
    printf("Testing width above range; expected: Cannot print binary; width is not between 1 and 32; actual:\n");
    print_binary(0x2C, 33);
    printf("\n");
}

static void test_get_field(void)
{
    test_summary_t summary = {0, 0};

    printf("=== get_field ===\n");
    // Check ordinary extraction and valid fields at the word boundaries.
    check_uint32(&summary, "extract bits 4-11", get_field(0xABCD, 4, 8),
        UINT32_C(0xBC));
    check_uint32(&summary, "width 1 at position 8",
        get_field(UINT32_C(0x12345678), 8, 1), UINT32_C(0));
    check_uint32(&summary, "width 32 at position 0",
        get_field(UINT32_C(0x89ABCDEF), 0, 32), UINT32_C(0x89ABCDEF));
    check_uint32(&summary, "position 31 with width 1",
        get_field(UINT32_C(0x89ABCDEF), 31, 1), UINT32_C(1));

    // Check that extraction preserves zero and all-one field values.
    check_uint32(&summary, "zero-valued field",
        get_field(UINT32_C(0x00000000), 4, 8), UINT32_C(0));
    check_uint32(&summary, "all-one field",
        get_field(UINT32_MAX, 4, 8), UINT32_C(0xFF));

    // Reject widths below 1 or above 32.
    check_uint32(&summary, "width above 32", get_field(0xABCD, 1, 33),
        UINT32_MAX);
    check_uint32(&summary, "width below 1",
        get_field(0xABCD, 0, -1), UINT32_MAX);

    // Reject positions outside 0 through 31.
    check_uint32(&summary, "position above 31", get_field(0xABCD, 32, 4),
        UINT32_MAX);
    check_uint32(&summary, "negative position",
        get_field(0xABCD, -1, 1), UINT32_MAX);

    // Reject valid-sized fields extending past bit 31.
    check_uint32(&summary, "position plus width above 32",
        get_field(0xABCD, 30, 4), UINT32_MAX);
    print_summary("get_field", summary);
}

static void test_set_field(void)
{
    test_summary_t summary = {0, 0};

    printf("=== set_field ===\n");
    // Check ordinary replacement and preservation of bits outside the field.
    check_uint32(&summary, "replace bits 8-15",
        set_field(UINT32_C(0x12345678), 8, 8, UINT32_C(0xA5)),
        UINT32_C(0x1234A578));

    // Check minimum and maximum widths and the highest valid position.
    check_uint32(&summary, "width 1 at position 0",
        set_field(UINT32_C(0x12345678), 0, 1, UINT32_C(1)),
        UINT32_C(0x12345679));
    check_uint32(&summary, "width 32 at position 0",
        set_field(UINT32_C(0x12345678), 0, 32, UINT32_C(0x89ABCDEF)),
        UINT32_C(0x89ABCDEF));
    check_uint32(&summary, "position 31 with width 1",
        set_field(UINT32_C(0x12345678), 31, 1, UINT32_C(1)),
        UINT32_C(0x92345678));

    // Check that values wider than the field are truncated to fit.
    check_uint32(&summary, "truncate value wider than field",
        set_field(UINT32_C(0x12345678), 8, 8, UINT32_C(0x1FF)),
        UINT32_C(0x1234FF78));

    // Reject widths outside 1 through 32.
    check_uint32(&summary, "width above 32",
        set_field(0x12345678, 8, 33, 0x1FF), UINT32_MAX);
    check_uint32(&summary, "zero width",
        set_field(0x12345678, 8, 0, 0x1FF), UINT32_MAX);
    check_uint32(&summary, "negative width",
        set_field(0x12345678, 8, -1, 0x1FF), UINT32_MAX);

    // Reject positions outside 0 through 31.
    check_uint32(&summary, "position above 31",
        set_field(0x12345678, 32, 1, 0x1FF), UINT32_MAX);
    check_uint32(&summary, "negative position",
        set_field(0x12345678, -1, 1, 0x1FF), UINT32_MAX);

    // Reject valid-sized fields extending past bit 31.
    check_uint32(&summary, "position plus width above 32",
        set_field(0x12345678, 12, 24, 0x1FF), UINT32_MAX);
    print_summary("set_field", summary);
}

static void test_sign_extend(void)
{
    test_summary_t summary = {0, 0};

    printf("=== sign_extend ===\n");
    // Check negative and positive results, full-width handling, and invalid widths.
    check_int32(&summary, "0xF8 = -8", sign_extend(0xF8, 8), -8);
    check_int32(&summary, "width 32 preserves 0xF8",
        sign_extend(0xF8, 32), UINT32_C(0xF8));
    check_int32(&summary, "width above 32",
        sign_extend(0xF8, 33), -1);
    check_int32(&summary, "zero width", sign_extend(0xF8, 0), -1);
    check_int32(&summary, "minimum 32-bit signed value",
        sign_extend(UINT32_C(0x80000000), 32), INT32_MIN);
    print_summary("sign_extend", summary);
}

static int statuses_equal(status_t actual, status_t expected)
{
    return actual.STATUS_HEAT == expected.STATUS_HEAT
        && actual.STATUS_COOL == expected.STATUS_COOL
        && actual.STATUS_FAN == expected.STATUS_FAN
        && actual.STATUS_FAULT == expected.STATUS_FAULT
        && actual.STATUS_MODE == expected.STATUS_MODE
        && actual.STATUS_RESERVED == expected.STATUS_RESERVED
        && actual.STATUS_SETPOINT == expected.STATUS_SETPOINT;
}

static void check_status_unpack(test_summary_t *summary, const char *name,
    uint16_t word, status_t expected)
{
    status_t actual = status_unpack(word);

    printf("Testing %s (0x%04" PRIX16 "):\n", name, word);
    status_print(actual);
    record_result(summary, name, statuses_equal(actual, expected));
    printf("\n");
}

static void test_status_unpack(void)
{
    test_summary_t summary = {0, 0};
    const status_t sample_expected = {
        .STATUS_HEAT = true,
        .STATUS_COOL = false,
        .STATUS_FAN = false,
        .STATUS_FAULT = false,
        .STATUS_MODE = STATUS_MODE_AUTO,
        .STATUS_RESERVED = false,
        .STATUS_SETPOINT = 22
    };
    const status_t negative_setpoint_expected = {
        .STATUS_HEAT = true,
        .STATUS_COOL = false,
        .STATUS_FAN = true,
        .STATUS_FAULT = true,
        .STATUS_MODE = STATUS_MODE_AUTO,
        .STATUS_RESERVED = false,
        .STATUS_SETPOINT = -8
    };
    const status_t invalid_mode_expected = {
        .STATUS_HEAT = false,
        .STATUS_COOL = true,
        .STATUS_FAN = false,
        .STATUS_FAULT = false,
        .STATUS_MODE = STATUS_MODE_INVALID,
        .STATUS_RESERVED = true,
        .STATUS_SETPOINT = 127
    };

    printf("=== status_unpack ===\n");
    // Check ordinary decoding, signed setpoints, and invalid mode handling.
    check_status_unpack(&summary, "sample status", UINT16_C(0x1631),
        sample_expected);
    check_status_unpack(&summary, "negative setpoint status", UINT16_C(0xF83D),
        negative_setpoint_expected);
    check_status_unpack(&summary, "invalid mode status", UINT16_C(0x7FD2),
        invalid_mode_expected);
    print_summary("status_unpack", summary);
}

int main(void)
{
    test_print_binary();
    test_get_field();
    test_set_field();
    test_sign_extend();
    test_status_unpack();

    printf("Overall automated checks: %u passed, %u failed\n",
        total_passed, total_failed);

    return total_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}