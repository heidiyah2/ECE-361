#include <stdio.h>
#include "check.h"
#include "status.h"

static void round_trip(status_fields_t in, const char *label) {
    char what[100];
    uint32_t word = 0;
    status_fields_t out = { 0 };

    snprintf(what, sizeof what, "round trip %s: pack returns 0", label);
    check(status_pack(&in, &word) == 0, what);
    snprintf(what, sizeof what, "round trip %s: unpack returns 0", label);
    check(status_unpack(word, &out) == 0, what);

    snprintf(what, sizeof what, "round trip %s: state", label);
    check(out.state == in.state, what);
    snprintf(what, sizeof what, "round trip %s: pump", label);
    check(out.pump == in.pump, what);
    snprintf(what, sizeof what, "round trip %s: heater", label);
    check(out.heater == in.heater, what);
    snprintf(what, sizeof what, "round trip %s: fan", label);
    check(out.fan == in.fan, what);
    snprintf(what, sizeof what, "round trip %s: paused", label);
    check(out.paused == in.paused, what);
}

static void test_round_trip(void) {
    /* pump, heater, fan, paused, state */
    round_trip((status_fields_t){ false, false, false, false, IDLE },    "IDLE");
    round_trip((status_fields_t){ true,  false, false, false, PREWASH }, "PREWASH, pump on");
    round_trip((status_fields_t){ true,  true,  false, false, WASH },    "WASH, pump and heater on");
    round_trip((status_fields_t){ false, false, false, true,  WASH },    "WASH, paused");
    round_trip((status_fields_t){ false, false, true,  false, DRY },     "DRY, fan on");
    round_trip((status_fields_t){ false, false, false, false, DONE },    "DONE");
}

static void test_unused_bits(void) {
    status_fields_t all_on = { true, true, true, true, DONE };
    uint32_t word = 0;

    status_pack(&all_on, &word);
    check((word & ~UINT32_C(0x7F)) == 0, "unused bits 7-31 are 0 with every field at its highest value");
}

int main(void) {
    test_round_trip();
    test_unused_bits();
    return check_done();
}
