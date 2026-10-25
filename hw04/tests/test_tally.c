#include "../tally.h"
#include <string.h>

static int passed = 0;
static int failed = 0;

// print PASS or FAIL for one check and count it
static void check(int ok, const char *what) {
    printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (ok) {
        passed++;
    } else {
        failed++;
    }
}

// print t into a temporary stream and compare against expected
static int prints_as(const tally_t *t, const char *expected) {
    char buf[512] = "";
    FILE *f = tmpfile();
    if (f == NULL) {
        return 0;
    }
    tally_print(t, f);
    rewind(f);
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    buf[n] = '\0';
    fclose(f);
    return strcmp(buf, expected) == 0;
}

static void test_counting_up(void) {
    tally_t *t = tally_create();
    check(tally_add(t, "door") == 1, "counting up: first add returns 1");
    check(tally_add(t, "door") == 2, "counting up: second add returns 2");
    check(tally_add(t, "door") == 3, "counting up: third add returns 3");
    check(tally_count(t, "door") == 3, "counting up: count is 3");
    check(tally_size(t) == 1, "counting up: size stays 1 for one name");
    tally_destroy(t);
}

static void test_never_added(void) {
    tally_t *t = tally_create();
    check(tally_count(t, "door") == 0, "never added: count 0 on empty tally");
    tally_add(t, "door");
    check(tally_count(t, "window") == 0, "never added: count 0 for other name");
    check(tally_size(t) == 1, "never added: count does not add the name");
    tally_destroy(t);
}

static void test_empty_name(void) {
    tally_t *t = tally_create();
    check(tally_add(t, "") == -1, "empty name: add returns -1");
    check(tally_size(t) == 0, "empty name: size unchanged");
    tally_destroy(t);
}

static void test_name_length(void) {
    tally_t *t = tally_create();
    char exact[TALLY_LEN + 1];
    char too_long[TALLY_LEN + 2];
    memset(exact, 'a', TALLY_LEN);
    exact[TALLY_LEN] = '\0';
    memset(too_long, 'b', TALLY_LEN + 1);
    too_long[TALLY_LEN + 1] = '\0';

    check(tally_add(t, exact) == 1, "length: TALLY_LEN chars accepted");
    check(tally_count(t, exact) == 1, "length: TALLY_LEN name stored whole");
    check(tally_add(t, too_long) == -1, "length: TALLY_LEN + 1 chars refused");
    check(tally_count(t, too_long) == 0, "length: refused name not stored");
    check(tally_size(t) == 1, "length: size unchanged after refusal");
    tally_destroy(t);
}

static void test_null_args(void) {
    tally_t *t = tally_create();
    tally_add(t, "door");
    check(tally_add(NULL, "door") == -1, "NULL: add with NULL tally returns -1");
    check(tally_add(t, NULL) == -1, "NULL: add with NULL name returns -1");
    check(tally_count(NULL, "door") == 0, "NULL: count with NULL tally returns 0");
    check(tally_count(t, NULL) == 0, "NULL: count with NULL name returns 0");
    check(tally_size(NULL) == 0, "NULL: size of NULL returns 0");
    tally_destroy(NULL);
    check(1, "NULL: destroy(NULL) does nothing");
    tally_print(NULL, stdout);
    tally_print(t, NULL);
    check(1, "NULL: print with NULL tally or stream does nothing");
    check(tally_count(t, "door") == 1, "NULL: real tally unchanged");
    tally_destroy(t);
}

static void test_full_tally(void) {
    tally_t *t = tally_create();
    char name[TALLY_LEN + 1];
    int all_added = 1;
    for (int i = 0; i < TALLY_NAMES; i++) {
        snprintf(name, sizeof name, "name%02d", i);
        if (tally_add(t, name) != 1) {
            all_added = 0;
        }
    }
    check(all_added, "full: TALLY_NAMES different names all added");
    check(tally_size(t) == TALLY_NAMES, "full: size is TALLY_NAMES");
    check(tally_add(t, "extra") == -1, "full: new name refused");
    check(tally_count(t, "extra") == 0, "full: refused name not stored");
    check(tally_size(t) == TALLY_NAMES, "full: size unchanged after refusal");
    check(tally_add(t, "name00") == 2, "full: existing name (first) still counts");
    check(tally_add(t, "name15") == 2, "full: existing name (last) still counts");
    tally_destroy(t);
}

static void test_pool_limit(void) {
    tally_t *t[TALLY_MAX];
    int all_created = 1;
    for (int i = 0; i < TALLY_MAX; i++) {
        t[i] = tally_create();
        if (t[i] == NULL) {
            all_created = 0;
        }
    }
    check(all_created, "pool: TALLY_MAX tallies created");
    int distinct = 1;
    for (int i = 0; i < TALLY_MAX; i++) {
        for (int j = i + 1; j < TALLY_MAX; j++) {
            if (t[i] == t[j]) {
                distinct = 0;
            }
        }
    }
    check(distinct, "pool: each tally is a different slot");

    tally_t *extra = tally_create();
    check(extra == NULL, "pool: one more than TALLY_MAX returns NULL");
    tally_destroy(extra);

    for (int i = 0; i < TALLY_MAX; i++) {
        tally_destroy(t[i]);
    }
}

static void test_create_after_destroy(void) {
    tally_t *t[TALLY_MAX];
    for (int i = 0; i < TALLY_MAX; i++) {
        t[i] = tally_create();
    }
    tally_add(t[1], "door");
    tally_add(t[1], "window");
    tally_destroy(t[1]);

    tally_t *again = tally_create();
    check(again != NULL, "reuse: create works after destroy on a full pool");
    check(tally_size(again) == 0, "reuse: reused tally starts empty");
    check(tally_count(again, "door") == 0, "reuse: old names are gone");

    tally_destroy(again);
    for (int i = 0; i < TALLY_MAX; i++) {
        if (i != 1) {
            tally_destroy(t[i]);
        }
    }
}

static void test_independent(void) {
    tally_t *a = tally_create();
    tally_t *b = tally_create();
    tally_add(a, "door");
    tally_add(a, "door");
    tally_add(b, "window");
    check(tally_count(a, "door") == 2, "independent: a counts its own name");
    check(tally_count(b, "door") == 0, "independent: b does not see a's name");
    check(tally_count(a, "window") == 0, "independent: a does not see b's name");
    check(tally_size(a) == 1 && tally_size(b) == 1, "independent: sizes are separate");
    tally_destroy(a);
    check(tally_count(b, "window") == 1, "independent: destroying a leaves b alone");
    tally_destroy(b);
}

static void test_print_order(void) {
    tally_t *t = tally_create();
    check(prints_as(t, ""), "print: empty tally prints nothing");

    tally_add(t, "light");   // into empty tally
    tally_add(t, "window");  // at the end
    tally_add(t, "door");    // at the front
    tally_add(t, "lamp");    // in the middle
    tally_add(t, "light");
    check(prints_as(t, "door 1\nlamp 1\nlight 2\nwindow 1\n"),
          "print: names in strcmp order with counts");

    // strcmp order puts uppercase before lowercase
    tally_add(t, "Zebra");
    check(prints_as(t, "Zebra 1\ndoor 1\nlamp 1\nlight 2\nwindow 1\n"),
          "print: uppercase sorts before lowercase");
    tally_destroy(t);
}

int main(void) {
    test_counting_up();
    test_never_added();
    test_empty_name();
    test_name_length();
    test_null_args();
    test_full_tally();
    test_pool_limit();
    test_create_after_destroy();
    test_independent();
    test_print_order();

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
