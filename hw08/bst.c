#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "bst.h"

/* from BinarySearchTree.c: one node of the tree */
typedef struct node {
   int data;

   struct node *leftChild;
   struct node *rightChild;
} node_t;

struct bst {
   struct node *root;
   size_t size;                         /* number of keys */
};

bst_t *bst_create(void) {
   bst_t *t = malloc(sizeof *t);
   if (t == NULL) {
      return NULL;
   }
   // make tree empty
   t->root = NULL;
   t->size = 0;
   return t;
}

/* helper function to bst_destroy */
static void destroy(node_t *n) {  // recursive
   if (n == NULL)             // base case
      return;
   destroy(n->leftChild);
   destroy(n->rightChild);
   free(n);
}

void bst_destroy(bst_t *t) {
   if (t == NULL) {
      return;                 // does nothing
   }
   destroy(t->root);
   free(t);
}

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
      // go to left tree
      if(current->data > data) {
         current = current->leftChild;
      }
      // else go to right tree
      else {
         current = current->rightChild;
      }

      // not found
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

size_t bst_size(const bst_t *t) {
   if (t == NULL)
      return 0;
   return t->size;
}

/* helper function for bst_height, recursive */
static size_t height(const node_t *n) {
   // base case, no node
   if (n == NULL)
      return 0;
   size_t left = height(n->leftChild);
   size_t right = height(n->rightChild);
   return 1 + (left > right ? left : right);
}

size_t bst_height(const bst_t *t) {
   if (t == NULL) 
      return 0;
   return height(t->root);
}
