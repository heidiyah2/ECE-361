#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "bst.h"

/* from BinarySearchTree.c: one node of the tree */
struct node {
   int data;

   struct node *leftChild;
   struct node *rightChild;
};

/* replaces the original's global `struct node *root`; each tree has its own */
struct bst {
   struct node *root;
   size_t size;                         /* number of keys */
};

/* new: the original's root started as NULL at file scope */
bst_t *bst_create(void) {
   return NULL;                         /* TODO */
}

/* new: the original never frees */
void bst_destroy(bst_t *t) {
   (void)t;                             /* TODO */
}

/* from insert(): root is now t->root; added the NULL t and malloc checks and
 * the size count, otherwise the original's loop */
int bst_insert(bst_t *t, int key) {
   if (t == NULL)
      return -1;

   int data = key;
   struct node *tempNode = (struct node*) malloc(sizeof(struct node));
   struct node *current;
   struct node *parent;

   if (tempNode == NULL)
      return -1;

   tempNode->data = data;
   tempNode->leftChild = NULL;
   tempNode->rightChild = NULL;

   //if tree is empty
   if(t->root == NULL) {
      t->root = tempNode;
   } else {
      current = t->root;
      parent = NULL;

      while(1) {
         parent = current;

         //go to left of the tree
         if(data < parent->data) {
            current = current->leftChild;

            //insert to the left
            if(current == NULL) {
               parent->leftChild = tempNode;
               break;
            }
         }  //go to right of the tree
         else {
            current = current->rightChild;

            //insert to the right
            if(current == NULL) {
               parent->rightChild = tempNode;
               break;
            }
         }
      }
   }

   t->size++;
   return 0;
}

/* from search(): ported faithfully, loop and all; the printf of each node
 * became a visit count. NOT YET FIXED (Part 2) */
bool bst_contains(const bst_t *t, int key, size_t *visits) {
   if (visits != NULL)
      *visits = 0;
   if (t == NULL)
      return false;

   int data = key;
   size_t count = 0;
   struct node *current = t->root;

   /* original from BinarySearchTree.c without printing */
   while(current->data != data) {
      if(current != NULL)
         count++;                       /* was: printf("%d ",current->data); */

      // data @ current node > data we are looking for
      // so we need to traverse the path where data < data current node
      //go to left tree
      if(current->data > data) {
         current = current->leftChild;
      }
      //else go to right tree
      else {
         current = current->rightChild;
      }

      //not found
      if(current == NULL) {
         if (visits != NULL)
            *visits = count;
         return false;
      }
   }

   count++;                             /* the node that matched */
   if (visits != NULL)
      *visits = count;
   return true;
}

/* new */
size_t bst_size(const bst_t *t) {
   (void)t;                             /* TODO */
   return 0;
}

/* new */
size_t bst_height(const bst_t *t) {
   (void)t;                             /* TODO */
   return 0;
}
