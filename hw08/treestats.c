#include <stdio.h>

#include "bst.h"

int main(void) {
   // create tree
   bst_t *t = bst_create();
   if (t == NULL) {
      fprintf(stderr, "Could not create tree\n");
      return 1;
   }
   // read input: scanf returns 1 for an integer, 0 for something that is
   // not a number, EOF at the end of the input
   int value;
   int r;
   while ((r = scanf("%d", &value)) == 1) {
      if (bst_insert(t, value) != 0) {
         fprintf(stderr, "out of memory after %zu values\n", bst_size(t));
         bst_destroy(t);    // free
         return 1;
      }
   }
   if (r == 0) {
      printf("not a number after %zu values\n", bst_size(t));
      bst_destroy(t);    // free
      return 1;
   }
   // r == EOF: the whole input was read
   if (bst_size(t) == 0)
      printf("empty tree\n");
   else
      printf("n %zu, height %zu\n", bst_size(t), bst_height(t));

   bst_destroy(t);    // free
   return 0;
}
