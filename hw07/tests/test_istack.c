#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "istack.h"

static int passed = 0;
static int failed = 0;

static void check(int ok, const char *what) {
    printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
    if (ok) {
        passed++;
    } else {
        failed++;
    }
}

/* creates a stack, or records a FAIL and returns NULL so the test can stop */
static istack_t *new_stack(const char *test) {
    istack_t *s = istack_create();
    if (s == NULL) {
        printf("FAIL: %s: istack_create returned NULL\n", test);
        failed++;
    }
    return s;
}

/* a new stack: count 0, empty, room for ISTACK_START */
static void test_new_stack(void) {
    istack_t *s = new_stack("new stack");
    if (s == NULL) {
        return;
    }
    check(istack_count(s) == 0, "new stack: count is 0");
    check(istack_is_empty(s), "new stack: is empty");
    check(istack_capacity(s) == ISTACK_START, "new stack: capacity is 8");
    istack_destroy(s);
}

/* pop and peek on an empty stack return -1 and leave *v alone */
static void test_empty_pop_peek(void) {
    istack_t *s = new_stack("empty pop/peek");
    if (s == NULL) {
        return;
    }
    int v = 42;
    int rc = istack_pop(s, &v);
    check(rc == -1 && v == 42, "empty: pop returns -1, *v unchanged");
    rc = istack_peek(s, &v);
    check(rc == -1 && v == 42, "empty: peek returns -1, *v unchanged");
    check(istack_count(s) == 0, "empty: count still 0 after failed pop");
    istack_destroy(s);
}

/* last in, first out */
static void test_lifo(void) {
    istack_t *s = new_stack("LIFO");
    if (s == NULL) {
        return;
    }
    istack_push(s, 1);
    istack_push(s, 2);
    istack_push(s, 3);
    int a = 0, b = 0, c = 0;
    istack_pop(s, &a);
    istack_pop(s, &b);
    istack_pop(s, &c);
    check(a == 3 && b == 2 && c == 1, "LIFO: push 1 2 3, pop 3 2 1");
    check(istack_is_empty(s), "LIFO: empty after popping all three");
    istack_destroy(s);
}

/* full at 8; the 9th push doubles to 16 and keeps every value;
   the 17th doubles again to 32 */
static void test_growth(void) {
    istack_t *s = new_stack("growth");
    if (s == NULL) {
        return;
    }
    for (int i = 0; i < 8; i++) {
        istack_push(s, i);
    }
    check(istack_count(s) == 8 && istack_capacity(s) == 8,
          "growth: full at 8, capacity still 8");

    int rc = istack_push(s, 8);
    check(rc == 0 && istack_capacity(s) == 16, "growth: 9th push doubles capacity to 16");

    bool ok = true;
    for (int i = 8; i >= 0; i--) {
        int v = -1;
        if (istack_pop(s, &v) != 0 || v != i) {
            ok = false;
        }
    }
    check(ok, "growth: all 9 values kept across the realloc");

    for (int i = 0; i < 17; i++) {
        istack_push(s, i);
    }
    check(istack_capacity(s) == 32, "growth: 17th push doubles again to 32");
    istack_destroy(s);
}

/* 1000 pushes, then 1000 pops in reverse order */
static void test_thousand(void) {
    istack_t *s = new_stack("1000");
    if (s == NULL) {
        return;
    }
    bool ok = true;
    for (int i = 0; i < 1000; i++) {
        if (istack_push(s, i) != 0) {
            ok = false;
        }
    }
    check(ok && istack_count(s) == 1000, "1000: all pushes succeed, count is 1000");

    ok = true;
    for (int i = 999; i >= 0; i--) {
        int v = -1;
        if (istack_pop(s, &v) != 0 || v != i) {
            ok = false;
        }
    }
    check(ok && istack_is_empty(s), "1000: pops return 999..0, then empty");
    istack_destroy(s);
}

/* emptying a stack and pushing again works */
static void test_reuse(void) {
    istack_t *s = new_stack("reuse");
    if (s == NULL) {
        return;
    }
    for (int i = 0; i < 10; i++) {
        istack_push(s, i);
    }
    /* pop exactly 10 times; a loop that waits for pop to fail would hang
       if pop never empties the stack */
    for (int i = 0; i < 10; i++) {
        istack_pop(s, NULL);
    }
    check(istack_is_empty(s), "reuse: empty after 10 pushes and 10 pops");
    istack_push(s, 7);
    istack_push(s, 8);
    int v = 0;
    istack_peek(s, &v);
    check(istack_count(s) == 2 && v == 8, "reuse: push after emptying, count 2, top 8");
    istack_destroy(s);
}

/* peek does not remove; peek and the following pop agree */
static void test_peek(void) {
    istack_t *s = new_stack("peek");
    if (s == NULL) {
        return;
    }
    istack_push(s, 10);
    istack_push(s, 20);
    int a = 0, b = 0;
    istack_peek(s, &a);
    istack_peek(s, &b);
    check(a == 20 && b == 20 && istack_count(s) == 2,
          "peek: twice gives the same value, count unchanged");

    int p = 0;
    istack_peek(s, &a);
    istack_pop(s, &p);
    check(a == p && istack_count(s) == 1, "peek: then pop returns the same value");
    istack_destroy(s);
}

/* is_empty agrees with count == 0 after each kind of operation */
static void test_is_empty_agrees(void) {
    istack_t *s = new_stack("is_empty");
    if (s == NULL) {
        return;
    }
    bool ok = true;
    ok = ok && (istack_is_empty(s) == (istack_count(s) == 0));     /* new */
    istack_push(s, 1);
    ok = ok && (istack_is_empty(s) == (istack_count(s) == 0));     /* push */
    istack_peek(s, NULL);
    ok = ok && (istack_is_empty(s) == (istack_count(s) == 0));     /* peek */
    istack_pop(s, NULL);
    ok = ok && (istack_is_empty(s) == (istack_count(s) == 0));     /* pop */
    istack_pop(s, NULL);
    ok = ok && (istack_is_empty(s) == (istack_count(s) == 0));     /* failed pop */
    for (int i = 0; i < 9; i++) {
        istack_push(s, i);
    }
    ok = ok && (istack_is_empty(s) == (istack_count(s) == 0));     /* after growth */
    check(ok, "is_empty: agrees with count == 0 after every operation");
    istack_destroy(s);
}

/* to_array: bottom first, a copy, more than 8 values, and still valid
   after the stack reallocs */
static void test_to_array(void) {
    istack_t *s = new_stack("to_array");
    if (s == NULL) {
        return;
    }
    for (int i = 0; i < 20; i++) {
        istack_push(s, i * 10);
    }
    size_t n = 0;
    int *a = istack_to_array(s, &n);
    if (a == NULL) {
        check(0, "to_array: returned NULL for 20 values");
        istack_destroy(s);
        return;
    }
    /* the rest of this test indexes a[0..19], so a wrong n must stop it
       here, before anything reads past the end of the array */
    if (n != 20) {
        check(0, "to_array: n is 20 for 20 values");
        free(a);
        istack_destroy(s);
        return;
    }
    bool ok = true;
    for (size_t i = 0; i < 20; i++) {
        if (a[i] != (int)i * 10) {
            ok = false;
        }
    }
    check(ok, "to_array: 20 values, bottom first, n is 20");

    a[19] = -1;                         /* change the copy ... */
    int top = 0;
    istack_peek(s, &top);
    check(top == 190, "to_array: is a copy (changing it leaves the stack alone)");

    /* push past capacity 32, so the stack's array moves; the copy must not */
    for (int i = 0; i < 20; i++) {
        istack_push(s, 999);
    }
    ok = true;
    for (size_t i = 0; i < 19; i++) {
        if (a[i] != (int)i * 10) {
            ok = false;
        }
    }
    check(ok && istack_capacity(s) == 64, "to_array: copy still intact after the stack reallocs");

    free(a);
    istack_destroy(s);
}

/* to_array on an empty stack: NULL and *n = 0 */
static void test_to_array_empty(void) {
    istack_t *s = new_stack("to_array empty");
    if (s == NULL) {
        return;
    }
    size_t n = 99;
    int *a = istack_to_array(s, &n);
    check(a == NULL && n == 0, "to_array: empty stack gives NULL and *n = 0");
    istack_destroy(s);
}

/* every function handles NULL arguments */
static void test_null_args(void) {
    int v = 42;
    check(istack_push(NULL, 1) == -1, "NULL: push returns -1");
    check(istack_pop(NULL, &v) == -1 && v == 42, "NULL: pop returns -1, *v unchanged");
    check(istack_peek(NULL, &v) == -1 && v == 42, "NULL: peek returns -1, *v unchanged");
    check(istack_count(NULL) == 0, "NULL: count is 0");
    check(istack_is_empty(NULL), "NULL: is_empty is true");
    check(istack_capacity(NULL) == 0, "NULL: capacity is 0");

    size_t n = 99;
    int *a = istack_to_array(NULL, &n);
    check(a == NULL && n == 0, "NULL: to_array gives NULL and *n = 0");

    istack_destroy(NULL);
    check(1, "NULL: destroy does nothing");

    istack_t *s = new_stack("NULL args");
    if (s == NULL) {
        return;
    }
    istack_push(s, 5);
    istack_push(s, 6);
    int rc = istack_peek(s, NULL);
    check(rc == 0 && istack_count(s) == 2, "NULL v: peek returns 0, count unchanged");
    rc = istack_pop(s, NULL);
    check(rc == 0 && istack_count(s) == 1, "NULL v: pop returns 0 and removes the top");
    a = istack_to_array(s, NULL);
    check(a != NULL && a[0] == 5, "NULL n: to_array still returns the array");
    free(a);
    istack_destroy(s);
}

/* two stacks do not share anything */
static void test_two_stacks(void) {
    istack_t *s = new_stack("two stacks");
    istack_t *t = new_stack("two stacks");
    if (s == NULL || t == NULL) {
        istack_destroy(s);
        istack_destroy(t);
        return;
    }
    for (int i = 0; i < 10; i++) {
        istack_push(s, i);
    }
    istack_push(t, 100);
    int vs = 0, vt = 0;
    istack_peek(s, &vs);
    istack_peek(t, &vt);
    check(istack_count(s) == 10 && istack_count(t) == 1 && vs == 9 && vt == 100,
          "two stacks: pushes to one do not show up in the other");
    check(istack_capacity(s) == 16 && istack_capacity(t) == 8,
          "two stacks: only the full one grew");
    istack_pop(t, NULL);
    check(istack_is_empty(t) && istack_count(s) == 10,
          "two stacks: popping one leaves the other alone");
    istack_destroy(s);
    istack_destroy(t);
}

/* destroying a stack that still holds values (and has grown);
   make memcheck reports a leak if anything is left behind */
static void test_destroy_nonempty(void) {
    istack_t *s = new_stack("destroy");
    if (s == NULL) {
        return;
    }
    for (int i = 0; i < 50; i++) {
        istack_push(s, i);
    }
    istack_destroy(s);
    check(1, "destroy: a stack holding 50 values (leaks checked by make memcheck)");
}

int main(void) {
    test_new_stack();
    test_empty_pop_peek();
    test_lifo();
    test_growth();
    test_thousand();
    test_reuse();
    test_peek();
    test_is_empty_agrees();
    test_to_array();
    test_to_array_empty();
    test_null_args();
    test_two_stacks();
    test_destroy_nonempty();

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
