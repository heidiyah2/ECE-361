#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

static int passed = 0;
static int failed = 0;

static void check(int ok, const char *what) {
   printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
   fflush(stdout);                      /* so earlier results survive a crash */
   if (ok) {
      passed++;
   } else {
      failed++;
   }
}

/* creates a tree, or records a FAIL and returns NULL so the test can stop */
static bst_t *new_tree(const char *test) {
   bst_t *t = bst_create();
   if (t == NULL) {
      printf("FAIL: %s: bst_create returned NULL\n", test);
      failed++;
   }
   return t;
}

/* the original's seven keys: size 7, height 3, 31 found in 3 visits,
 * 15 not found in 3 */
static void test_seven_keys(void) {
   int keys[] = { 27, 14, 35, 10, 19, 31, 42 };
   size_t visits = 99;
   bst_t *t = new_tree("seven keys");
   if (t == NULL)
      return;

   for (int i = 0; i < 7; i++)
      bst_insert(t, keys[i]);

   check(bst_size(t) == 7, "seven keys: size 7");
   check(bst_height(t) == 3, "seven keys: height 3");

   bool found = bst_contains(t, 31, &visits);
   check(found && visits == 3, "seven keys: 31 found in 3 visits");

   visits = 99;
   found = bst_contains(t, 15, &visits);
   check(!found && visits == 3, "seven keys: 15 not found in 3 visits");

   bst_destroy(t);
}

/* inserting the same key twice keeps both: size 2 */
static void test_duplicate(void) {
   bst_t *t = new_tree("duplicate");
   if (t == NULL)
      return;

   int r1 = bst_insert(t, 5);
   int r2 = bst_insert(t, 5);
   check(r1 == 0 && r2 == 0, "duplicate: both inserts return 0");
   check(bst_size(t) == 2, "duplicate: size 2");

   bst_destroy(t);
}

/* keys in increasing order make a straight line: height equals their number */
static void test_increasing(void) {
   const int n = 100;
   bst_t *t = new_tree("increasing");
   if (t == NULL)
      return;

   for (int i = 1; i <= n; i++)
      bst_insert(t, i);

   check(bst_size(t) == (size_t)n, "increasing: size 100");
   check(bst_height(t) == (size_t)n, "increasing: height 100");

   bst_destroy(t);
}

/* NULL tree: every function does what Table 1 says for NULL */
static void test_null_tree(void) {
   size_t visits = 99;
   check(bst_insert(NULL, 1) == -1, "NULL tree: insert returns -1");
   check(!bst_contains(NULL, 1, &visits) && visits == 0,
         "NULL tree: contains is false, 0 visits");
   check(bst_size(NULL) == 0, "NULL tree: size 0");
   check(bst_height(NULL) == 0, "NULL tree: height 0");
   bst_destroy(NULL);                   /* must not crash */
   check(1, "NULL tree: destroy does nothing");
}

/* destroying a full tree: frees every node and the tree. The sanitizer
 * build (make memcheck) is the judge: it reports a leak or a double free */
static void test_destroy_full(void) {
   int keys[] = { 27, 14, 35, 10, 19, 31, 42 };
   bst_t *t = new_tree("destroy full tree");
   if (t == NULL)
      return;

   for (int i = 0; i < 7; i++)
      bst_insert(t, keys[i]);

   bst_destroy(t);
   check(1, "destroy full tree: no crash (leaks checked by make memcheck)");
}

/* the input that crashed the faithful port: searching an empty tree.
 * Run last, because a crash ends the program */
static void test_empty_search(void) {
   size_t visits = 99;
   bst_t *t = new_tree("empty search");
   if (t == NULL)
      return;

   bool found = bst_contains(t, 5, &visits);
   check(!found && visits == 0, "empty search: 5 not found in 0 visits");
   check(bst_size(t) == 0 && bst_height(t) == 0, "empty search: size 0, height 0");

   bst_destroy(t);
}

int main(void) {
   test_seven_keys();
   test_duplicate();
   test_increasing();
   test_null_tree();
   test_destroy_full();
   test_empty_search();

   printf("%d passed, %d failed\n", passed, failed);
   return failed == 0 ? 0 : 1;
}
