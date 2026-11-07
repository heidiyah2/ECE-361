#include <stdio.h>
#include <stdlib.h>
#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

#define TMP_SCENARIO "test_scenario.tmp"

static int passed = 0;
static int failed = 0;
static uint32_t *base = NULL;

static void check(int ok, const char *what) {
    printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (ok) {
        passed++;
    } else {
        failed++;
    }
}

static int load_text(const char *text, scenario_t *s) {
    FILE *f = fopen(TMP_SCENARIO, "w");
    if (f == NULL) {
        return -2;
    }
    fputs(text, f);
    fclose(f);
    int result = scenario_load(TMP_SCENARIO, s);
    remove(TMP_SCENARIO);
    return result;
}

static uint32_t leds_after_led(uint32_t start, int n, int on, int *ret) {
    int rc;
    iom361_writeReg(base, LEDS_REG, start, &rc);
    *ret = panel_led(base, n, on);
    return iom361_readReg(base, LEDS_REG, &rc);
}

static uint32_t leds_after_toggle(uint32_t start, int n, int *ret) {
    int rc;
    iom361_writeReg(base, LEDS_REG, start, &rc);
    *ret = panel_toggle(base, n);
    return iom361_readReg(base, LEDS_REG, &rc);
}

static void test_sensor_failed_read(void) {
    reading_t r = {123, -1.0f, -2.0f};
    int ret = sensor_read(base, 1791273600, &r);
    check(ret != 0, "sensor: read before iom361_initialize fails");
    check(r.t == 123 && r.temp_c == -1.0f && r.rh_pct == -2.0f,
          "sensor: failed read changes nothing");
}

static void test_sensor(void) {
    reading_t r;

    check(sensor_read(base, 1791273600, &r) == 0 && r.temp_c == 23.5f && r.rh_pct == 75.0f,
          "sensor: default after init is 23.5 C, 75.0 %RH");
    check(r.t == 1791273600, "sensor: timestamp now is stored");

    _iom361_set_sensor1(91.0f, 40.0f);
    check(sensor_read(base, 0, &r) == 0 && r.temp_c == 91.0f && r.rh_pct == 40.0f,
          "sensor: 91.0 C reads back as 91.0");

    _iom361_set_sensor1(22.04f, 41.96f);
    check(sensor_read(base, 0, &r) == 0 && r.temp_c == 22.0f && r.rh_pct == 42.0f,
          "sensor: 22.04 / 41.96 round to 22.0 / 42.0");
}

static void test_pressed_released(void) {
    check(panel_pressed(0x1, 0x0) == 0x1 && panel_released(0x1, 0x0) == 0x0,
          "panel: a press is pressed, not released");
    check(panel_pressed(0x0, 0x1) == 0x0 && panel_released(0x0, 0x1) == 0x1,
          "panel: a release is released, not pressed");
    check(panel_pressed(0x1, 0x1) == 0x0 && panel_released(0x1, 0x1) == 0x0,
          "panel: a held switch is neither");
    check(panel_pressed(0x6, 0x3) == 0x4 && panel_released(0x6, 0x3) == 0x1,
          "panel: 0x3 -> 0x6 presses switch 2, releases switch 0, holds switch 1");
}

static void test_panel_led(void) {
    int ret;

    check(leds_after_led(0xA, 2, 1, &ret) == 0xE && ret == 0,
          "panel_led: LED 2 on keeps the other LEDs (0xA -> 0xE)");
    check(leds_after_led(0xA, 3, 0, &ret) == 0x2 && ret == 0,
          "panel_led: LED 3 off keeps the other LEDs (0xA -> 0x2)");
    check(leds_after_led(0x0, 0, 1, &ret) == 0x1 && ret == 0,
          "panel_led: LED 0 on is bit 0");
    check(leds_after_led(0x0, 31, 1, &ret) == 0x80000000 && ret == 0,
          "panel_led: LED 31 on is bit 31");
    check(leds_after_led(0xA, 32, 1, &ret) == 0xA && ret == -1,
          "panel_led: n = 32 returns -1, LEDs unchanged");
    check(leds_after_led(0xA, -1, 1, &ret) == 0xA && ret == -1,
          "panel_led: n = -1 returns -1, LEDs unchanged");
}

static void test_panel_toggle(void) {
    int ret;

    check(leds_after_toggle(0xA, 2, &ret) == 0xE && ret == 0,
          "panel_toggle: LED 2 off -> on keeps the other LEDs (0xA -> 0xE)");
    check(leds_after_toggle(0xA, 3, &ret) == 0x2 && ret == 0,
          "panel_toggle: LED 3 on -> off keeps the other LEDs (0xA -> 0x2)");
    check(leds_after_toggle(0xA, 32, &ret) == 0xA && ret == -1,
          "panel_toggle: n = 32 returns -1, LEDs unchanged");

    int rc;
    iom361_writeReg(base, LEDS_REG, 0x5, &rc);
    panel_toggle(base, 1);
    panel_toggle(base, 1);
    check(iom361_readReg(base, LEDS_REG, &rc) == 0x5,
          "panel_toggle: toggling twice restores the LEDs");
}

static void test_scenario_load(void) {
    static scenario_t s;

    int ret = load_text("# a comment\n"
                        "\n"
                        "start 1791273600   # 2026-10-06 08:00:00 UTC\n"
                        "# tick temp rh switches\n"
                        "0   21.5  40.0  0x0\n"
                        "\n"
                        "5   22.0  41.0  0x1   # pressed\n"
                        "60  56.0  80.0  0xA\n", &s);
    check(ret == 0 && s.count == 3 && s.start == 1791273600,
          "load: comments and blank lines are skipped");
    check(ret == 0 && s.lines[2].tick == 60 && s.lines[2].temp_c == 56.0f
          && s.lines[2].rh_pct == 80.0f && s.lines[2].switches == 0xA,
          "load: fields of a line are read, switches in hex");

    check(load_text("# no start\n0 20.0 40 0x0\n", &s) == 2,
          "load: no start line is wrong at the first data line (2)");
    check(load_text("start 1791273600\n0 20.0 40 0x0\n5 20.0 40\n", &s) == 3,
          "load: missing field is wrong at that line (3)");
    check(load_text("start 1791273600\n0 20.0 40 0x0\n5 20.0 40 0x1\n3 20.0 40 0x0\n", &s) == 4,
          "load: decreasing tick is wrong at that line (4)");
    check(load_text("start 1791273600\n0 20.0 40 0x0\n5 20.0 40 0x1\n5 20.0 40 0x0\n", &s) == 4,
          "load: repeated tick is wrong at that line (4)");
    check(scenario_load("no_such_file.txt", &s) == -1,
          "load: a file that can't be opened returns -1");
    check(load_text("start 1791273600\n# nothing else\n", &s) == 3,
          "load: start with no data is wrong at the line after the last (3)");
    check(load_text("", &s) == 1,
          "load: an empty file is wrong at line 1");
    check(load_text("start 1791273600\n2 20.0 40 0x0\n", &s) == 2,
          "load: first tick not 0 is wrong at that line (2)");
    check(load_text("start 1791273600\n0 20.0 40 0x0 0x1\n", &s) == 2,
          "load: extra field is wrong at that line (2)");
    check(load_text("start 1791273600\n0 20.0 40 0x0\nstart 5\n", &s) == 3,
          "load: second start line is wrong at that line (3)");
}

static void test_scenario_at(void) {
    static scenario_t s;
    int ret = load_text("start 1791273600\n"
                        "0   20.0  40  0x0\n"
                        "5   21.0  41  0x1\n"
                        "60  22.0  42  0x0\n", &s);
    check(ret == 0, "at: scenario with lines at 0, 5, 60 loads");
    if (ret != 0) {
        return;
    }

    const scenario_line_t *l3 = scenario_at(&s, 3);
    const scenario_line_t *l59 = scenario_at(&s, 59);
    check(l3 != NULL && l3->tick == 0 && l3->temp_c == 20.0f,
          "at: values hold between lines (tick 3 -> line 0)");
    check(l59 != NULL && l59->tick == 5 && l59->switches == 0x1,
          "at: values hold between lines (tick 59 -> line 5)");

    const scenario_line_t *l5 = scenario_at(&s, 5);
    check(l5 != NULL && l5->tick == 5, "at: tick 5 is line 5 (a line is in force at its own tick)");

    const scenario_line_t *l500 = scenario_at(&s, 500);
    check(l500 != NULL && l500->tick == 60, "at: after the last line, the last line holds");

    check(scenario_at(&s, -1) == NULL, "at: before the first line is NULL");
}

int main(void) {
    test_sensor_failed_read();

    int rc;
    base = iom361_initialize(4, 4, &rc);
    if (base == NULL) {
        printf("FAIL: iom361_initialize returned %d\n", rc);
        return EXIT_FAILURE;
    }
    iom361_set_display(false);

    test_sensor();
    test_pressed_released();
    test_panel_led();
    test_panel_toggle();
    test_scenario_load();
    test_scenario_at();

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
